# Change these
cxxtestpath = /info/cprog11/cxxtest/
testcode    = example_test.h

tests: runtest01 runtest02 runtest03 runtest04 runtest05 runtest06 runtest07 runtest08 runtest09

.cpp.o:
	g++ -Wall -Wextra -Weffc++ -g -c $< -o $@

OurMatrix.o: OurMatrix.h

testcode.cpp: $(testcode)
	python $(cxxtestpath)cxxtestgen.py --error-printer -o testcode.cpp $(testcode)

runtest0%: testcode.cpp
	g++ -I $(cxxtestpath) -o runtest0$* testcode.cpp Matrix0$*.o

test: tests
	./runtest01 || true
	./runtest02 || true
	./runtest03 || true
	./runtest04 || true
	./runtest05 || true
	./runtest06 || true
	./runtest07 || true
	./runtest08 || true
	./runtest09 || true

runourtest: testcode.cpp OurMatrix.o ../1.5/vector.h
	g++ -I $(cxxtestpath) -g -o runourtest testcode.cpp OurMatrix.o

clean:
	-rm -f runtest0* runourtest testcode.cpp OurMatrix.o

