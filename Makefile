APP_NAME = lab3_app
TEST_NAME = lab3_tests

.PHONY: all tests run run_tests clean

all: $(APP_NAME)

$(APP_NAME): main.o
	g++ main.o -o $(APP_NAME)

main.o: main.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp SquareMatrix.hpp
	g++ -c main.cpp

tests: $(TEST_NAME)

$(TEST_NAME): tests.o
	g++ tests.o -o $(TEST_NAME)

tests.o: tests.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Bit.hpp BitSequence.hpp SequenceTests.hpp BitTests.hpp Matrix.hpp ScalarTraits.hpp RectangularMatrix.hpp SquareMatrix.hpp MatrixTests.hpp
	g++ -c tests.cpp

run: $(APP_NAME)
	./$(APP_NAME)

run_tests: $(TEST_NAME)
	./$(TEST_NAME)

clean:
	del /Q *.o 2>NUL
	del /Q $(APP_NAME).exe 2>NUL
	del /Q $(TEST_NAME).exe 2>NUL
