all: parser_test

parser_test: initial_test.o names.o parser.o scanner.o monitor.o devices.o
	g++ initial_test.o names.o parser.o scanner.o monitor.o devices.o -o parser_test

initial_test.o: initial_test.cc
	g++ -c initial_test.cc

names.o: ../../names.cc
	g++ -c ../../names.cc

parser.o: ../../parser.cc
	g++ -c ../../parser.cc

monitor.o: ../../monitor.cc
	g++ -c ../../monitor.cc

devices.o: ../../devices.cc
	g++ -c ../../devices.cc
	
scanner.o: ../../scanner.cc
	g++ -c ../../scanner.cc
	
clean:
	rm -rf *o parser_test
