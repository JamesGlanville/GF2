all: parser_test

parser_test: initial_test.o names.o parser.o scanner.o test_file_parser.o
	g++ initial_test.o names.o parser.o scanner.o test_file_parser.o -o parser_test

initial_test.o: initial_test.cc parser.o scanner.o names.o test_file_parser.o
	g++ -c initial_test.cc

names.o: ../../names.cc 
	g++ -c ../../names.cc

parser.o: ../../parser.cc
	g++ -c ../../parser.cc
	
scanner.o: ../../scanner.cc
	g++ -c ../../scanner.cc
	
test_file_parser.o: ../../test_file_parser.cc
	g++ -c ../../test_file_parser.cc

clean:
	rm -rf *o parser_test

