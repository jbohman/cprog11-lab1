
CXXTESTPATH=../../cxxtest/
CXXTESTGEN=$(CXXTESTPATH)cxxtestgen.py


all: test_vec test_template_vec our_tests

test_template_vec.o: vector.h
test_vec.o: vector.h

%.o: %.cpp
	g++ -c -Wall -Wextra -g $*.cpp

# old test program
test_template_vec: test_template_vec.o vector.h
	g++ -g -Wall -Wextra test_template_vec.o -o test_template_vec

test_vec: test_vec.o vector.h
	g++ -g -Wall -Wextra test_vec.o -o test_vec

# our own test code
our_tests_gen.cpp: our_tests.cpp vector.h
	$(CXXTESTGEN) --error-printer -o $@ our_tests.cpp

our_tests: our_tests_gen.cpp our_tests.cpp vector.h
	g++ -g -o our_tests -I $(CXXTESTPATH) our_tests_gen.cpp our_tests.cpp

# i/o wrapper used by kattis
kattis_wrapper:
	g++ -g -Wextra -Wall ../../info-lab1/cprog11lab15.cpp -I. -o kattis_wrapper


clean:
	-$(RM) test_vec test_template_vec

.PHONY: clean

