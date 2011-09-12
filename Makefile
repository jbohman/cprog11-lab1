%.out:	%.cpp
	g++ -g -Wall $*.cpp -o $*.out
