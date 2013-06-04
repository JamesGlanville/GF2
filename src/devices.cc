#include "devices.h"
#include <iostream>
#include <string>
#include <cstdlib> 

using namespace std;

/***********************************************************************
 *
 * Used to print out signal values for debugging in showdevice.
 *
 */
void devices::outsig (asignal s)
{
  switch (s) {
    case high:    cout << "high";    break;
    case low:     cout << "low";     break;
    case rising:  cout << "rising";  break;
    case falling: cout << "falling"; break;
  }
}


/***********************************************************************
 *
 * Used to print out device details and signal values 
 * for debugging in executedevices.
 *
 */
void devices::showdevice (devlink d)
{
  inplink  i;
  outplink o;
  cout << "   Device: ";
  nmz->writename (d->id);
  cout << "  Kind: ";
  writedevice (d->kind);
  cout << endl;
  cout << "   Inputs:" << endl;
  for (i = d->ilist; i != NULL; i = i->next) {
    cout << "      ";
    nmz->writename (i->id);
    cout << " ";
    outsig (i->connect->sig);
    cout << endl;
  }
  cout << "   Outputs:";
  for (o = d->olist; o != NULL; o = o->next) {
    cout << "      ";
    nmz->writename (o->id);
    cout << " ";
    outsig (o->sig);
    cout << endl;
  }
  cout << endl;
}


/***********************************************************************
 *
 * Sets the state of the named switch. 'ok' returns false if switch  
 * not found.                                                        
 *
 */
void devices::setswitch (name sid, asignal level, bool& ok)
{
  devlink d;
  d = netz->finddevice (sid);
  ok = (d != NULL);
  if (ok) {
    ok = (d->kind == aswitch);
    if (ok)
      d->swstate = level;
  }
}

/***********************************************************************
 *
 * Gets the state of the named switch. 'ok' returns false if switch  
 * not found.                                                        
 *
 */
void devices::getswitchstate (name sid, asignal& level, bool& ok)
{
  devlink d;
  d = netz->finddevice (sid);
  ok = (d != NULL);
  if (ok) {
    ok = (d->kind == aswitch);
    if (ok)
       level = d->swstate;
  }
}

/***********************************************************************
 *
 * Used to make new switch devices.
 * Called by makedevice.
 *
 */
void devices::makeswitch (name id, int setting, bool& ok)
{
  devlink d;
  ok = (setting <= 1);
  if (ok) {
    netz->adddevice (aswitch, id, d);
    netz->addoutput (d, blankname);
    d->swstate = (setting == 0) ? low : high;
  }
}


/***********************************************************************
 *
 * Used to make new clock devices.
 * Called by makedevice.
 *
 */
void devices::makeclock (name id, int frequency)
{
  devlink d;
  netz->adddevice (aclock, id, d);
  netz->addoutput (d, blankname);
  d->frequency = frequency;
  d->counter = 0;
}


/***********************************************************************
 *
 * Used to make new AND, NAND, OR, NOR and XOR gates. 
 * Called by makedevice.
 *
 */
void devices::makegate (devicekind dkind, name did, int ninputs, bool& ok)
{
  const int maxinputs = 16;
  devlink d;
  int n;
  namestring iname;
  ok = (ninputs <= maxinputs);
  if (ok) {
    netz->adddevice (dkind, did, d);
    netz->addoutput (d, blankname);
    for (n = 1; n <= ninputs; n++) {
      iname = "i";
      if (n < 10) {
	iname += ((char) n) + '0';
      } else {
	iname += ((char) (n / 10)) + '0';
	iname += ((char) (n % 10)) + '0';
      }
      netz->addinput (d, nmz->lookup (iname));
    }
  }
}


/***********************************************************************
 *
 * Used to make new D-type bistable devices.
 * Inputs: D, clock, preset and clear.
 * Outputs: Q, QBAR.
 * Called by makedevice.
 *
 */
void devices::makedtype (name id)
{
  devlink d;
  netz->adddevice (dtype, id, d);
  netz->addinput (d, datapin);
  netz->addinput (d, clkpin);
  netz->addinput (d, setpin);
  netz->addinput (d, clrpin);
  netz->addoutput (d, qpin);
  netz->addoutput (d, qbarpin);
  d->memory = low;
}


/***********************************************************************
 *
 * Adds a device to the network of the specified kind and name.  The  
 * variant is used with such things as gates where it specifies the   
 * number of inputs. 'ok' returns true if operation succeeds.         
 *
 */
void devices::makedevice (devicekind dkind, name did, int variant, bool& ok)
{
  ok = true;
  switch (dkind) {
    case aswitch:
      makeswitch (did, variant, ok);
      break;
    case aclock:
      makeclock (did, variant);
      break;
    case andgate:
    case nandgate:
    case orgate:
    case norgate:
      makegate (dkind, did, variant, ok);
      break;
    case xorgate:
      makegate (dkind, did, 2, ok);
      break;
    case dtype:
      makedtype(did);
      break;
  }
}


/***********************************************************************
 *
 * Update signal `sig' in the direction of signal `target'.
 * Set steadystate to false if this results in a change in sig.
 *
 */
void devices::signalupdate (asignal target, asignal& sig)
{
  asignal oldsig;
  oldsig = sig;
  switch (sig) {
    case falling:
    case low:
      sig = (target == high) ? rising : low;
      break;
    case rising:
    case high:
      sig = (target == low) ? falling : high;
      break;
  }
  if (sig != oldsig)
    steadystate = false;
}


/***********************************************************************
 *
 * Returns the inverse of a signal.
 *
 */
asignal devices::inv (asignal s)
{
  return ((s == high) ? low : high);
}


/***********************************************************************
 *
 * Used to simulate the operation of switch devices.
 * Called by executedevices.
 *
 */
void devices::execswitch (devlink d)
{
  signalupdate (d->swstate, d->olist->sig);
}


/***********************************************************************
 *
 * Used to simulate the operation of AND, OR, NAND and NOR gates.
 * Called by executedevices.
 * Meaning of arguments: gate output is 'y' iff all inputs are 'x'
 *
 */
void devices::execgate (devlink d, asignal x, asignal y)
{
  asignal newoutp;
  inplink inp = d->ilist;
  outplink outp = d->olist;
  newoutp = y;
  while ((inp != NULL) && (newoutp == y)) {
    if (inp->connect->sig == inv (x))
      newoutp = inv (y);
    inp = inp->next;
  }
  signalupdate (newoutp, outp->sig);
}


/***********************************************************************
 *
 * Used to simulate the operation of exclusive or gates.
 * Called by executedevices.
 *
 */
void devices::execxorgate(devlink d)
{
  asignal newoutp;
  if (d->ilist->connect->sig == d->ilist->next->connect->sig)
    newoutp = low;
  else
    newoutp = high;
  signalupdate (newoutp, d->olist->sig);
}


/***********************************************************************
 *
 * Used to simulate the operation of D-type bistables.
 * Called by executedevices. The signal on the data input
 * immediately BEFORE the clock edge is transferred to the
 * Q output. We are effectively assuming a negligible but
 * nonzero setup time, and a zero hold time.
 *
 */
void devices::execdtype (devlink d, int machinecycle)
{
 static bool complete;
 static bool hasrisen;
 asignal RAND = low;
 if (rand()&0x01){RAND=high;}
 //cout << "RAND: " << RAND <<endl;
 const int holdtime = 10; //Number of loops to try and get steadystate in.
 asignal datainput, clkinput, setinput, clrinput;
 inplink i;
 outplink qout, qbarout;
 i = netz->findinput (d, datapin); datainput = i->connect->sig;
 i = netz->findinput (d, clkpin);  clkinput  = i->connect->sig;
 i = netz->findinput (d, clrpin);  clrinput  = i->connect->sig;
 i = netz->findinput (d, setpin);  setinput  = i->connect->sig;
 qout = netz->findoutput (d, qpin);
 qbarout = netz->findoutput (d, qbarpin);
	   
  if (machinecycle ==1) {complete = false;hasrisen=false;}
  if (!complete)
  {
	  //cout <<"cycle: " << machinecycle << " id: " << d->id<<endl;
	  if (clkinput == rising) {hasrisen = true;}
	  
	  if (hasrisen && (datainput == falling || datainput == rising))
	  {
		signalupdate(RAND,qout->sig);
		signalupdate(inv (RAND), qbarout->sig);
		cout << "Warning: DTYPE output is random." <<endl;
		complete=true;
		return;
	  }
	  
	  if (hasrisen && datainput == high)
	  {
		   //cout << "MEMORY SET HIGH" << endl;
		d->memory = high;
	  }
	  if (hasrisen && datainput == low)
	  {
		  // cout << "MEMORY SET LOW" << endl;
		d->memory = low;
	  }
	  if (setinput == high)
		{d->memory = high;
		/*cout << "MEMORY SET HIGHby setinput" << endl;*/}

	  if (clrinput == high)
		{//cout << "MEMORY SET LOWby setinput" << endl;
		d->memory = low;}
			  
	  if (machinecycle <= holdtime)
	  {
		steadystate = false;
	  }
	  else
	  {
		  complete = true;
	  }
  }
  else
  {
  
  }
	//  cout << "DOING SIGNAL UPDATE WITH MEM= " << d->memory <<" for dev: "<<d->id<<endl;
  signalupdate (d->memory, qout->sig);
  signalupdate (inv (d->memory), qbarout->sig);
}

/*******
 * 
 * Used to simulate the operation of SIGGEN devices.
 * 
 * 
 */
 
void devices::execsiggen(devlink d, int machinecycle)
{
	if (machinecycle == 1){
		
	if (d->dataloc >= d->data.size()-1)
	{
		d->dataloc = 0;
	}
	else
	{
		d->dataloc++;
	}}
	
	if (!d->data[d->dataloc])
	{
		cout <<"HIGH" <<endl;
		signalupdate(high,d->olist->sig);
	}
	else
	{
		cout << "LOW" <<endl;
		signalupdate(low,d->olist->sig);
	}
}

void devices::makesiggen (name did, vector <bool> data)
{
  devlink d;
  netz->adddevice (siggen, did, d);
  netz->addoutput (d, blankname);
  d->dataloc = 0;
  d->data = data;
}

/***********************************************************************
 *
 * Used to simulate the operation of clock devices.
 * Called by executedevices.
 *
 */
void devices::execclock(devlink d)
{
  if (d->olist->sig == rising)
    signalupdate (high, d->olist->sig);
  else {
    if (d->olist->sig == falling)
      signalupdate (low, d->olist->sig);
  }
}


/***********************************************************************
 *
 * Increment the counters in the clock devices and initiate changes
 * in their outputs when the end of their period is reached.
 * Called by executedevices.
 *
 */
void devices::updateclocks (void)
{
  devlink d;
  for (d = netz->devicelist (); d != NULL; d = d->next) {
    if (d->kind == aclock) {
      if (d->counter == d->frequency) {
	d->counter = 0;
	if (d->olist->sig == high)
	  d->olist->sig = falling;
	else
	  d->olist->sig = rising;
      }
      (d->counter)++;
    }
  }
}


/***********************************************************************
 *
 * Executes all devices in the network to simulate one complete clock 
 * cycle. 'ok' is returned false if network fails to stabilise (i.e.  
 * it is oscillating).                                            
 *
 */
void devices::executedevices (bool& ok)
{
  const int maxmachinecycles = 20;
  devlink d;
  vector <devlink> d_vector;
  int machinecycle;
  if (debugging)
    cout << "Start of execution cycle" << endl;
  updateclocks ();
  machinecycle = 0;
  do {
    machinecycle++;
    if (debugging)
      cout << "machine cycle # " << machinecycle << endl;
    steadystate = true;
    for (d = netz->devicelist (); d != NULL; d = d->next) {
      switch (d->kind) {
        case aswitch:  execswitch (d);           break; //Switches and clocks should be updated first?
        case aclock:   execclock (d);            break;
       }
      if (debugging)
	  showdevice (d);
  }
    for (d = netz->devicelist (); d != NULL; d = d->next) {
		d_vector.push_back(d);
	}
	std::random_shuffle ( d_vector.begin(), d_vector.end() );
	for (int i=0;i<d_vector.size();i++){
		d = d_vector[i];
      switch (d->kind) {
       case orgate:   execgate (d, low, low);   break;
        case norgate:  execgate (d, low, high);  break;
        case andgate:  execgate (d, high, high); break;
        case nandgate: execgate (d, high, low);  break;
        case xorgate:  execxorgate (d);          break;
        case dtype:    execdtype (d,machinecycle);            break;     
        case siggen:   execsiggen (d,machinecycle);            break;     
     } 
      
    }
  } while ((! steadystate) && (machinecycle < maxmachinecycles));
  if (debugging)
    cout << "End of execution cycle" << endl;
  ok = steadystate;
}


/***********************************************************************
 *
 * Prints out the given device kind. 
 * Used by showdevice.
 *
 */
void devices::writedevice (devicekind k)
{
  nmz->writename (dtab[k]);
}


/***********************************************************************
 *
 * Returns the kind of device corresponding to the given name.   
 * 'baddevice' is returned if the name is not a legal device.    
 *
 */
devicekind devices::devkind (name id)
{
  devicekind d;
  d = aswitch;
  while ((d != baddevice) && (dtab[d] != id)) 
    d = static_cast<devicekind>(d + 1);
  return (d);
}


/***********************************************************************
 *
 * Set the state of the internal debugging flag.
 *
 */
void devices::debug (bool on)
{
  debugging = on;
}


/***********************************************************************
 * 
 * Constructor for the devices class.
 * Registers the names of all the possible devices.
 * 
 */
devices::devices (names* names_mod, network* net_mod, devicetable* dt_mod)
{
  nmz = names_mod;
  netz = net_mod;
  dtz = dt_mod;
  dtab[aswitch]   =  nmz->lookup("switch");
  dtab[aclock]    =  nmz->lookup("clock");
  dtab[andgate]   =  nmz->lookup("and");
  dtab[nandgate]  =  nmz->lookup("nand");
  dtab[orgate]    =  nmz->lookup("or");
  dtab[norgate]   =  nmz->lookup("nor");
  dtab[xorgate]   =  nmz->lookup("xor");
  dtab[dtype]     =  nmz->lookup("dtype");
  dtab[siggen]     =  nmz->lookup("siggen");
  dtab[baddevice] =  blankname;
  debugging = false;
  datapin = nmz->lookup("data");
  clkpin  = nmz->lookup("clk");
  setpin  = nmz->lookup("set");
  clrpin  = nmz->lookup("clear");
  qpin    = nmz->lookup("q");
  qbarpin = nmz->lookup("qbar");
}

/***********************************************************************
 * 
 * Allows the GUI to access the switch names
 * 
 */
string devices::getswitch(int swnum)
{
  return dtz->getswitch(swnum);
}


bool devices::lookup(int id, namestring& str, device_type& type, numinputs& numinput)
{
	return dtz->lookup( id,  str,  type,  numinput);
}
