#include "gtest/gtest.h"
#include <infra/DoubleLinkedList.hpp>

template<typename T>
struct TestLinkedListCreator : DoubleLinkedList<T>
{
    TestLinkedListCreator(std::initializer_list<T>&& list)
        : datas(list)
    {
        items.reserve(count);

        for_each(datas.begin(), datas.end(), [this](auto& data) { items.emplace_back(data); });

        for_each(items.begin(), items.end(), [this](auto& item) { this->AddBack(item); });
    }

    std::vector<T> datas;
    std::vector<TestLinkedListCreator::Item> items;
};

template<typename T>
struct AddBack
{
    AddBack(T data, DoubleLinkedList<T>& list)
        : data(data)
        , item(this->data)
    {
        list.AddBack(this->item);
    }

    T data;
    typename DoubleLinkedList<T>::Item item;
};

template<typename T>
struct AddFront
{
    AddFront(T data, DoubleLinkedList<T>& list)
        : data(data)
        , item(this->data)
    {
        list.AddFront(this->item);
    }

    T data;
    typename DoubleLinkedList<T>::Item item;
};

TEST(TestDoubleLinkedList, Size)
{
    TestLinkedListCreator<uint32_t> list_of_4({1, 2, 3, 4});

    EXPECT_EQ(4, list_of_4.Size());

    TestLinkedListCreator<uint32_t> list_of_10({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    EXPECT_EQ(10, list_of_10.Size());
}

TEST(TestDoubleLinkedList, GivenAnEmptyList_WhenConstructed_ThenEmptyShouldReturnTrue)
{
    DoubleLinkedList<uint32_t> list;

    EXPECT_TRUE(list.Empty());
}

TEST(TestDoubleLinkedList, GivenAnEmptyList_WhenValueAddedToBack_ThenEmptyShouldReturnFalse)
{
    DoubleLinkedList<uint32_t> list;

    ASSERT_TRUE(list.Empty());

    AddBack<uint32_t> addBack_5(5, list);

    EXPECT_FALSE(list.Empty());
}

TEST(TestDoubleLinkedList, GivenAnEmptyList_WhenValueAddedToFront_ThenEmptyShouldReturnFalse)
{
    DoubleLinkedList<uint32_t> list;

    ASSERT_TRUE(list.Empty());

    AddFront<uint32_t> addFront_5(5, list);

    EXPECT_FALSE(list.Empty());
}

TEST(TestDoubleLinkedList, GivenAnNonEmptyList_WhenAllValuesRemovedUsingPopBack_ThenEmptyShouldReturnTrue)
{
    TestLinkedListCreator<uint32_t> list({1, 2, 3});

    ASSERT_FALSE(list.Empty());

    list.PopBack();
    ASSERT_FALSE(list.Empty());

    list.PopBack();
    ASSERT_FALSE(list.Empty());

    list.PopBack();
    EXPECT_TRUE(list.Empty());
}

TEST(TestDoubleLinkedList, GivenAnNonEmptyList_WhenAllValuesRemovedUsingPopFront_ThenEmptyShouldReturnTrue)
{
    TestLinkedListCreator<uint32_t> list({1, 2, 3});

    ASSERT_FALSE(list.Empty());

    list.PopFront();
    ASSERT_FALSE(list.Empty());

    list.PopFront();
    ASSERT_FALSE(list.Empty());

    list.PopFront();
    EXPECT_TRUE(list.Empty());
}

TEST(TestDoubleLinkedList, AddBack)
{
}

TEST(TestDoubleLinkedList, GivenAnEmptyList_WhenItemAddedUsingAddBack_ThenTheItemsInternalValuesAreNullptr)
{
    DoubleLinkedList<uint32_t> list;

    AddBack<uint32_t> addBack_5(5, list);

    EXPECT_EQ(nullptr, addBack_5.item.GetNext());
    EXPECT_EQ(nullptr, addBack_5.item.GetPrevious());
}

TEST(TestDoubleLinkedList, GivenAnEmptyList_WhenItemAddedUsingAddFront_ThenTheItemsInternalValuesAreNullptr)
{
    DoubleLinkedList<uint32_t> list;

    AddFront<uint32_t> addFront_5(5, list);

    EXPECT_EQ(nullptr, addFront_5.item.GetNext());
    EXPECT_EQ(nullptr, addFront_5.item.GetPrevious());
}

TEST(TestDoubleLinkedList, AddFront)
{
}

TEST(TestDoubleLinkedList, InsertInfront)
{
    DoubleLinkedList<uint32_t> list;

    AddBack<uint32_t> addBack_1(3, list);
    AddBack<uint32_t> addBack_2(5, list);
    AddBack<uint32_t> addBack_3(7, list);

    uint32_t data;
    decltype(list)::Item item(data);

    ASSERT_EQ(nullptr, item.GetNext());
    ASSERT_EQ(nullptr, item.GetPrevious());

    list.InsertInfront(addBack_2.item, item);

    EXPECT_EQ(&addBack_1.item, item.GetNext());
    EXPECT_EQ(&addBack_2.item, item.GetPrevious());

    EXPECT_EQ(&item, addBack_1.item.GetPrevious());
    EXPECT_EQ(&item, addBack_2.item.GetNext());
}

TEST(TestDoubleLinkedList, InsertBehind)
{
}

TEST(TestDoubleLinkedList, Remove)
{
}

TEST(TestDoubleLinkedList, PopBack)
{
}

TEST(TestDoubleLinkedList, PopFront)
{
}

TEST(TestDoubleLinkedList, PeekBack)
{
}

TEST(TestDoubleLinkedList, PeekFront)
{
}