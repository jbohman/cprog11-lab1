
CXXTESTPATH=../../cxxtest/
CXXTESTGEN=$(CXXTESTPATH)cxxtestgen.py

all: test_1.2b.out

%.o: %.cpp
	g++ -c $*.cpp

1.2b.cpp: test_must_follow_a.cpp
	$(CXXTESTGEN) --error-printer -o 1.2b.cpp test_must_follow_a.cpp

test_1.2b.out: 1.2b.cpp must_follow_a.o
	g++ -o test_1.2b.out -I $(CXXTESTPATH) 1.2b.cpp must_follow_a.o


test: all
	./test_1.2b.out


