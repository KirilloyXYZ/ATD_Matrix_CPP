all: lab3_app

lab3_app: main.o LinearSystemSolvers.o
	g++ main.o LinearSystemSolvers.o -o lab3_app

main.o: main.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp SquareMatrix.hpp LinearSystemSolvers.hpp
	g++ -c main.cpp

tests: tests.o LinearSystemSolvers.o
	g++ tests.o LinearSystemSolvers.o -o tests

tests.o: tests.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Bit.hpp BitSequence.hpp SequenceTests.hpp BitTests.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp SquareMatrix.hpp MatrixTests.hpp LinearSystemSolvers.hpp LinearSystemTests.hpp
	g++ -c tests.cpp

LinearSystemSolvers.o: LinearSystemSolvers.cpp LinearSystemSolvers.hpp Exceptions.h DynamicArray.hpp Sequence.hpp ArraySequence.hpp MutableArraySequence.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp
	g++ -c LinearSystemSolvers.cpp

run: lab3_app
	./lab3_app

run_tests: tests
	./tests

clean:
	del /Q *.o 2>NUL
	del /Q lab3_app.exe 2>NUL
	del /Q tests.exe 2>NUL
