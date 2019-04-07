#include "gtest/gtest.h"
#include <infra/SortedDoubleLinkedList.hpp>
#include "harness/CriticalSectionHarness.hpp"

auto predicate_smaller = [](DoubleLinkedList<uint32_t>::Item& currentItem, DoubleLinkedList<uint32_t>::Item& newItem) {
    return (*newItem < *currentItem);
};

using SortedSmallerDLL = SortedDoubleLinkedList<uint32_t, decltype(predicate_smaller)>;

TEST(TestSortedDoubleLinkedList_Insert, TestName)
{
    CriticalSectionSpy criticalSpy;

    SortedSmallerDLL sortedList(predicate_smaller);

    uint32_t data1 = 1;
    uint32_t data5 = 5;
    uint32_t data3 = 3;

    SortedSmallerDLL::Item item1{data1};
    SortedSmallerDLL::Item item5{data5};
    SortedSmallerDLL::Item item3{data3};

    sortedList.Insert(item3);

	ASSERT_EQ(&item3, sortedList.PeekFront());

    sortedList.Insert(item5);

	ASSERT_EQ(&item3, sortedList.PeekFront());

	sortedList.Insert(item1);

	EXPECT_EQ(&item1, sortedList.PeekFront());
}