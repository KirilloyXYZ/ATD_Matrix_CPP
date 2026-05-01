#include <cassert>
#include <iostream>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "SequenceTests.hpp"
#include "BitTests.hpp"
#include "MatrixTests.hpp"
#include "LinearSystemTests.hpp"

void TestDynamicArray()
{
    int data[] = {1, 2, 3};
    DynamicArray<int> arr(data, 3);

    assert(arr.GetSize() == 3);
    assert(arr.Get(0) == 1);
    assert(arr.Get(2) == 3);

    arr.Set(1, 10);
    assert(arr.Get(1) == 10);

    arr.Resize(5);
    assert(arr.GetSize() == 5);

    bool thrown = false;
    try
    {
        arr.Get(10);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    DynamicArray<int> grown(0);
    grown.PushBack(1);
    grown.PushBack(2);
    grown.PushBack(3);
    grown.PushBack(4);
    grown.PushBack(5);

    assert(grown.GetSize() == 5);
    assert(grown.Get(0) == 1);
    assert(grown.Get(1) == 2);
    assert(grown.Get(2) == 3);
    assert(grown.Get(3) == 4);
    assert(grown.Get(4) == 5);

    grown.InsertAt(0, 99);
    assert(grown.GetSize() == 6);
    assert(grown.Get(0) == 99);
    assert(grown.Get(1) == 1);

    grown.InsertAt(3, 77);
    assert(grown.GetSize() == 7);
    assert(grown.Get(0) == 99);
    assert(grown.Get(1) == 1);
    assert(grown.Get(2) == 2);
    assert(grown.Get(3) == 77);
    assert(grown.Get(4) == 3);
    assert(grown.Get(5) == 4);
    assert(grown.Get(6) == 5);

    thrown = false;
    try
    {
        grown.InsertAt(100, 1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    DynamicArray<int> resized(data, 3);
    resized.Resize(2);
    assert(resized.GetSize() == 2);
    assert(resized.Get(0) == 1);
    assert(resized.Get(1) == 2);

    DynamicArray<int> assigned(0);
    assigned = resized;
    resized.Set(0, 100);

    assert(assigned.GetSize() == 2);
    assert(assigned.Get(0) == 1);
    assert(assigned.Get(1) == 2);

    thrown = false;
    try
    {
        DynamicArray<int> badSize(-1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        DynamicArray<int> badItems(nullptr, 2);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        arr.Resize(-1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

void TestLinkedList()
{
    int data[] = {10, 20, 30};
    LinkedList<int> list(data, 3);

    assert(list.GetLength() == 3);
    assert(list.GetFirst() == 10);
    assert(list.GetLast() == 30);

    list.Append(40);
    list.Prepend(5);
    list.InsertAt(15, 1);

    assert(list.GetLength() == 6);
    assert(list.GetFirst() == 5);
    assert(list.Get(1) == 15);
    assert(list.GetLast() == 40);

    bool thrown = false;
    LinkedList<int> empty;
    try
    {
        empty.GetFirst();
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        empty.GetLast();
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        list.Get(100);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        list.GetSubList(3, 1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        list.Concat(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

int main()
{
    TestDynamicArray();
    TestLinkedList();

    TestMutableArraySequence();
    TestImmutableArraySequence();
    TestListSequence();
    TestSequenceAlgorithms();
    TestSequenceErrorHandling();

    TestBit();
    TestBitSequence();
    TestBitSequenceErrors();

    TestRectangularMatrixCreation();
    TestRectangularMatrixCopy();
    TestRectangularMatrixOperations();
    TestRectangularMatrixRowAndColumnOperations();
    TestSquareMatrix();
    TestMatrixErrors();

    TestGaussSolvers();
    TestGaussPartialPivot();
    TestLUDecompositionAndSolve();
    TestLinearSystemErrors();

    std::cout << "All tests passed!\n";
    return 0;
}
