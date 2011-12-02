all: test_vec 

test_vec.o: bitvector.h

%.o: %.cpp
	g++ -c -Wall -Wextra -std=c++0x -g $*.cpp

test_vec: test_vec.o bitvector.h
	g++ -g -Wall -Wextra test_vec.o -std=c++0x -o test_vec

clean:
	-$(RM) test_vec

.PHONY: clean

