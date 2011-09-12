
all: A.out Data.out

%.out: %.cpp
	g++ -g -Wall $*.cpp -o $*.out

vg-A: A.out
	valgrind --tool=memcheck --leak-check=yes ./A.out

vg-Data: Data.out
	valgrind --tool=memcheck --leak-check=yes ./Data.out

