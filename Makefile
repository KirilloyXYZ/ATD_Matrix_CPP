all: lab3_app

lab3_app: main.o LinearSystemSolvers.o
	g++ main.o LinearSystemSolvers.o -o lab3_app

main.o: main.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp SquareMatrix.hpp LinearSystemSolvers.hpp
	g++ -c main.cpp

tests: tests.o SequenceTests.o BitTests.o Bit.o BitSequence.o MatrixTests.o LinearSystemTests.o LinearSystemSolvers.o
	g++ tests.o SequenceTests.o BitTests.o Bit.o BitSequence.o MatrixTests.o LinearSystemTests.o LinearSystemSolvers.o -o tests

tests.o: tests.cpp DynamicArray.hpp LinkedList.hpp SequenceTests.hpp BitTests.hpp MatrixTests.hpp LinearSystemTests.hpp
	g++ -c tests.cpp

SequenceTests.o: SequenceTests.cpp SequenceTests.hpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp
	g++ -c SequenceTests.cpp

BitTests.o: BitTests.cpp BitTests.hpp Exceptions.h DynamicArray.hpp Sequence.hpp SequenceAlgorithms.hpp Bit.hpp BitSequence.hpp
	g++ -c BitTests.cpp

Bit.o: Bit.cpp Bit.hpp
	g++ -c Bit.cpp

BitSequence.o: BitSequence.cpp BitSequence.hpp Bit.hpp Exceptions.h DynamicArray.hpp Sequence.hpp
	g++ -c BitSequence.cpp

MatrixTests.o: MatrixTests.cpp MatrixTests.hpp Exceptions.h DynamicArray.hpp Sequence.hpp ArraySequence.hpp MutableArraySequence.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp SquareMatrix.hpp
	g++ -c MatrixTests.cpp

LinearSystemTests.o: LinearSystemTests.cpp LinearSystemTests.hpp LinearSystemSolvers.hpp Exceptions.h DynamicArray.hpp Sequence.hpp ArraySequence.hpp MutableArraySequence.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp
	g++ -c LinearSystemTests.cpp

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
