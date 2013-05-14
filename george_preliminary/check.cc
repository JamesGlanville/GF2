#include <iostream>
#include "check.h"
void check::ckstr(string str)
{
  for(int i=0; i <many; i++)
    if(str == badar[i])
      cout << "Checker detected bad word: " << str << endl;
}

check::check (void)
{
  badar[0] = "bad";
  badar[1] = "horrid";
  badar[2] = "ghastly";
}
