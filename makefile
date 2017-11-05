objects = arithmetic.o parse.o syntax-tree.o

calc : calc.cpp $(objects)

arithmetic.o : arithmetic.cpp arithmetic.h
parse.o : parse.cpp parse.h
syntax-tree.o : syntax-tree.cpp syntax-tree.h

.PHONY : clean
clean :
	rm calc $(objects)
