CXXTESTPATH=../../cxxtest/
CXXTESTGEN=$(CXXTESTPATH)cxxtestgen.py

all: test_vec our_tests

test_vec.o: bitvector.h

%.o: %.cpp
	g++ -c -Wall -Wextra -std=c++0x -g $*.cpp

test_vec: test_vec.o bitvector.h
	g++ -g -Wall -Wextra test_vec.o -std=c++0x -o test_vec

our_tests_gen.cpp: our_tests.cpp bitvector.h
	$(CXXTESTGEN) --error-printer -o $@ our_tests.cpp

our_tests: our_tests_gen.cpp our_tests.cpp bitvector.h
	g++ -g -std=c++0x -o our_tests -I $(CXXTESTPATH) our_tests_gen.cpp our_tests.cpp

clean:
	-$(RM) test_vec test_vec.o

.PHONY: clean

