all: test_vec 

test_vec.o: vector.h

%.o: %.cpp
	g++ -c -Wall -Wextra -g $*.cpp

test_vec: test_vec.o vector.h
	g++ -g -Wall -Wextra test_vec.o -o test_vec

clean:
	-$(RM) test_vec

.PHONY: clean

