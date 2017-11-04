objects = arithmetic.o parse.o syntax-tree.o

calc : calc.cpp $(objects)
	c++ -std=c++17 -o calc calc.cpp $(objects)

arithmetic.o : arithmetic.cpp arithmetic.h
	c++ -std=c++17 -c arithmetic.cpp

parse.o : parse.cpp parse.h
	c++ -std=c++17 -c parse.cpp

syntax-tree.o : syntax-tree.cpp syntax-tree.h
	c++ -std=c++17 -c syntax-tree.cpp

.PHONY : clean
clean :
	rm calc $(objects)
