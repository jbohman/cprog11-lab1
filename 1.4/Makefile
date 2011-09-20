
CXXTESTPATH=../../cxxtest/
CXXTESTGEN=$(CXXTESTPATH)cxxtestgen.py


all: test_vec our_tests

%.o: %.cpp
	g++ -c $*.cpp

# old test program
test_vec: test_vec.o vector.o vector.h
	g++ -g -Wall test_vec.o vector.o -o test_vec

# our own test code
our_tests_gen.cpp: our_tests.cpp vector.h
	$(CXXTESTGEN) --error-printer -o $@ our_tests.cpp

our_tests: our_tests_gen.cpp our_tests.cpp vector.h vector.o
	g++ -o our_tests -I $(CXXTESTPATH) our_tests_gen.cpp our_tests.cpp vector.o



clean:
	-$(RM) test_vec

.PHONY: clean

