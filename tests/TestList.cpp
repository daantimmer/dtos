//#include "gtest/gtest.h"
//#include <infra/List.hpp>
//
//template<typename T>
//struct ListCreator : List<T>
//{
//    ListCreator(std::initializer_list<T>&& list)
//        : creatorDatas(list)
//    {
//        creatorItems.reserve(list.size());
//
//        for_each(creatorDatas.begin(), creatorDatas.end(), [this](auto& data) {
//            creatorItems.emplace_back(data);
//
//            AddBack(creatorItems.back());
//        });
//    }
//
//    std::vector<T> creatorDatas;
//    std::vector<ListCreator::Item> creatorItems;
//};
//
//template<typename T>
//struct AddBack
//{
//    AddBack(T data, List<T>& list)
//        : data(data)
//        , item(this->data)
//    {
//        list.AddBack(this->item);
//    }
//
//    T data;
//    typename List<T>::Item item;
//};
//
//template<typename T>
//struct AddFront
//{
//    AddFront(T data, List<T>& list)
//        : data(data)
//        , item(this->data)
//    {
//        list.AddFront(this->item);
//    }
//
//    T data;
//    typename List<T>::Item item;
//};
//
//TEST(TestListCreator, WhenCreatedWithSingleItem_ThenTheSizeIsOneAndFrontAndBackAreSameItem)
//{
//    ListCreator<uint32_t> list{1};
//
//    EXPECT_EQ(1, list.Size());
//    EXPECT_EQ(&list.creatorItems[0], list.PeekFront());
//    EXPECT_EQ(&list.creatorItems[0], list.PeekBack());
//}
//
//TEST(TestListCreator, WhenCreatedWithMultipleItems_ThenTheSizeIsThreeAndFrontAndBackAreDifferntItems)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    EXPECT_EQ(3, list.Size());
//    EXPECT_EQ(&list.creatorItems[0], list.PeekFront());
//    EXPECT_EQ(&list.creatorItems[2], list.PeekBack());
//}
//
//TEST(TestList_Size, GivenMultipleSizedLists_WhenNothingChanges_ThenTheSizeShouldBeCorrect)
//{
//    ListCreator<uint32_t> list_of_4{1, 2, 3, 4};
//
//    EXPECT_EQ(4, list_of_4.Size());
//
//    ListCreator<uint32_t> list_of_10{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//
//    EXPECT_EQ(10, list_of_10.Size());
//}
//
//TEST(TestList_Empty, GivenAnEmptyList_WhenConstructed_ThenEmptyShouldReturnTrue)
//{
//    List<uint32_t> list;
//
//    EXPECT_TRUE(list.Empty());
//}
//
//TEST(TestList_Empty, GivenAnEmptyList_WhenValueAddedToBack_ThenEmptyShouldReturnFalse)
//{
//    List<uint32_t> list;
//
//    ASSERT_TRUE(list.Empty());
//
//    AddBack<uint32_t> addBack_5(5, list);
//
//    EXPECT_FALSE(list.Empty());
//}
//
//TEST(TestList_Empty, GivenAnEmptyList_WhenValueAddedToFront_ThenEmptyShouldReturnFalse)
//{
//    List<uint32_t> list;
//
//    ASSERT_TRUE(list.Empty());
//
//    AddFront<uint32_t> addFront_5(5, list);
//
//    EXPECT_FALSE(list.Empty());
//}
//
//TEST(TestList_Empty, GivenAnNonEmptyList_WhenAllValuesRemovedUsingPopBack_ThenEmptyShouldReturnTrue)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_FALSE(list.Empty());
//
//    list.PopBack();
//    ASSERT_FALSE(list.Empty());
//
//    list.PopBack();
//    ASSERT_FALSE(list.Empty());
//
//    list.PopBack();
//    EXPECT_TRUE(list.Empty());
//}
//
//TEST(TestList_Empty, GivenAnNonEmptyList_WhenAllValuesRemovedUsingPopFront_ThenEmptyShouldReturnTrue)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_FALSE(list.Empty());
//
//    list.PopFront();
//    ASSERT_FALSE(list.Empty());
//
//    list.PopFront();
//    ASSERT_FALSE(list.Empty());
//
//    list.PopFront();
//    EXPECT_TRUE(list.Empty());
//}
//
//TEST(TestList_AddBack,
//	GivenAEmptyList_WhenItemAddedUsingAddBack_ThenItemsInternalsAreReset)
//{
//    List<uint32_t> list;
//
//	AddBack<uint32_t> addBack(0, list);
//
//    EXPECT_EQ(&list, addBack.item.GetList());
//    EXPECT_EQ(nullptr, addBack.item.GetNext());
//    EXPECT_EQ(nullptr, addBack.item.GetPrevious());
//}
//
//TEST(TestList_AddBack,
//     GivenAListWithOneItem_WhenItemAddedUsingAddBack_ThenTheItemsInternalValuesAreSetCorrect)
//{
//    ListCreator<uint32_t> list{1};
//
//    AddBack<uint32_t> addBack(3, list);
//
//    EXPECT_EQ(&list.creatorItems[0], addBack.item.GetNext());
//    EXPECT_EQ(nullptr, addBack.item.GetPrevious());
//}
//
//TEST(TestList_AddBack, GivenAnEmptyList_WhenItemAddedUsingAddBack_ThenTheItemsInternalValuesAreNullptr)
//{
//    List<uint32_t> list;
//
//    AddBack<uint32_t> addBack_5(5, list);
//
//    EXPECT_EQ(nullptr, addBack_5.item.GetNext());
//    EXPECT_EQ(nullptr, addBack_5.item.GetPrevious());
//}
//
////TEST(TestList_AddBack, GivenAEmptyList_WhenItemAddedUsingAddBack_ThenItemsInternalsAreReset)
////{
////    List<uint32_t> list;
////
////    AddBack<uint32_t> addBack(0, list);
////
////    EXPECT_EQ(&list, addBack.item.GetList());
////    EXPECT_EQ(nullptr, addBack.item.GetNext());
////    EXPECT_EQ(nullptr, addBack.item.GetPrevious());
////}
//
//TEST(TestList_AddFront,
//     GivenAListWithOneItem_WhenItemAddedUsingAddFront_ThenTheItemsInternalValuesAreSetCorrect)
//{
//    ListCreator<uint32_t> list{2};
//
//    AddFront<uint32_t> addFront(3, list);
//
//    EXPECT_EQ(nullptr, addFront.item.GetNext());
//    EXPECT_NE(nullptr, addFront.item.GetPrevious());
//}
//
//TEST(TestList_AddFront, GivenAnEmptyList_WhenItemAddedUsingAddFront_ThenTheItemsInternalValuesAreNullptr)
//{
//    List<uint32_t> list;
//
//    AddFront<uint32_t> addFront_5(5, list);
//
//    EXPECT_EQ(nullptr, addFront_5.item.GetNext());
//    EXPECT_EQ(nullptr, addFront_5.item.GetPrevious());
//}
//
//TEST(TestList_InsertInfront,
//     GivenAListWithThreeItems_WhenItemAddedIsInFrontAnotherItem_ThenTheItemIsInfrontTheOtherItem)
//{
//    List<uint32_t> list;
//
//    AddBack<uint32_t> addBack_1(3, list);
//    AddBack<uint32_t> addBack_2(5, list);
//    AddBack<uint32_t> addBack_3(7, list);
//
//    ASSERT_EQ(3, list.Size());
//
//    uint32_t data;
//    decltype(list)::Item item(data);
//
//    ASSERT_EQ(nullptr, item.GetNext());
//    ASSERT_EQ(nullptr, item.GetPrevious());
//
//    list.InsertInfront(addBack_2.item, item);
//
//    EXPECT_EQ(4, list.Size());
//
//    EXPECT_EQ(&addBack_1.item, item.GetNext());
//    EXPECT_EQ(&addBack_2.item, item.GetPrevious());
//
//    EXPECT_EQ(&item, addBack_1.item.GetPrevious());
//    EXPECT_EQ(&item, addBack_2.item.GetNext());
//}
//
//TEST(TestList_InsertInfront, GivenAListWithASingleItem_WhenItemIsAddedInfront_ThenTheItemBecomesTheNewFront)
//{
//    ListCreator<uint32_t> list{1};
//
//    auto currentFront = list.PeekFront();
//    ASSERT_NE(nullptr, currentFront);
//
//    uint32_t data;
//    decltype(list)::Item item(data);
//
//    list.InsertInfront(*currentFront, item);
//
//    auto newFront = list.PeekFront();
//    ASSERT_NE(currentFront, newFront);
//
//    EXPECT_EQ(&item, newFront);
//}
//
//TEST(TestList_InsertBehind,
//     GivenAListWithThreeItems_WhenItemAddedIsBehindAnotherItem_ThenTheItemIsBehindTheOtherItem)
//{
//    List<uint32_t> list;
//
//    AddBack<uint32_t> addBack_1(3, list);
//    AddBack<uint32_t> addBack_2(5, list);
//    AddBack<uint32_t> addBack_3(7, list);
//
//    uint32_t data;
//    decltype(list)::Item item(data);
//
//    ASSERT_EQ(nullptr, item.GetNext());
//    ASSERT_EQ(nullptr, item.GetPrevious());
//
//    list.InsertBehind(addBack_2.item, item);
//
//    EXPECT_EQ(&addBack_2.item, item.GetNext());
//    EXPECT_EQ(&addBack_3.item, item.GetPrevious());
//
//    EXPECT_EQ(&item, addBack_2.item.GetPrevious());
//    EXPECT_EQ(&item, addBack_3.item.GetNext());
//}
//
//TEST(TestList_InsertBehind, GivenAListWithASingleItem_WhenItemIsAddedBehind_ThenTheItemBecomesTheNewBack)
//{
//    ListCreator<uint32_t> list{1};
//
//    auto currentBack = list.PeekBack();
//    ASSERT_NE(nullptr, currentBack);
//
//    uint32_t data;
//    decltype(list)::Item item(data);
//
//    list.InsertBehind(*currentBack, item);
//
//    auto newBack = list.PeekBack();
//    ASSERT_NE(currentBack, newBack);
//
//    EXPECT_EQ(&item, newBack);
//}
//
//TEST(TestList_Remove, GivenAListWithASingleItem_WhenItemIsRemoved_ThenTheListIsEmpty)
//{
//    ListCreator<uint32_t> list{1};
//
//    ASSERT_NE(nullptr, list.PeekBack());
//
//    list.Remove(*list.PeekBack());
//
//    EXPECT_TRUE(list.Empty());
//    EXPECT_EQ(0, list.Size());
//}
//
//TEST(TestList_Remove,
//     GivenAListWithMultipleItems_WhenItemIsRemovedFromFront_ThenTheSizeDecreasedByOneAndNotEmpty)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_FALSE(list.Empty());
//    ASSERT_EQ(3, list.Size());
//
//    list.Remove(*list.PeekFront());
//
//    ASSERT_FALSE(list.Empty());
//    ASSERT_EQ(2, list.Size());
//}
//
//TEST(TestList_Remove,
//     GivenAListWithMultipleItems_WhenItemIsRemovedFromBack_ThenTheSizeDecreasedByOneAndNotEmpty)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_FALSE(list.Empty());
//    ASSERT_EQ(3, list.Size());
//
//    list.Remove(*list.PeekBack());
//
//    ASSERT_FALSE(list.Empty());
//    ASSERT_EQ(2, list.Size());
//}
//
//TEST(TestList_Remove,
//     GivenAListWithMultipleItems_WhenItemIsRemovedFromMiddle_ThenTheSizeDecreasedByOneAndNotEmpty)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_FALSE(list.Empty());
//    ASSERT_EQ(3, list.Size());
//
//    list.Remove(list.creatorItems[1]);
//
//    EXPECT_FALSE(list.Empty());
//    EXPECT_EQ(2, list.Size());
//}
//
//TEST(TestList_Remove, GivenAListWithMultipleItems_WhenItemIsRemovedFromFront_ThenTheFrontIsUpdated)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_EQ(&list.creatorItems[0], list.PeekFront());
//
//    list.Remove(*list.PeekFront());
//
//    EXPECT_EQ(&list.creatorItems[1], list.PeekFront());
//}
//
//TEST(TestList_Remove, GivenAListWithMultipleItems_WhenItemIsRemovedFromBack_ThenTheBackisUpdated)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_EQ(&list.creatorItems[2], list.PeekBack());
//
//    list.Remove(*list.PeekBack());
//
//    EXPECT_EQ(&list.creatorItems[1], list.PeekBack());
//}
//
//TEST(TestList_Remove,
//     GivenAListWithMultipleItems_WhenItemIsRemovedFromMiddle_ThenTheFrontAndBackisNotUpdated)
//{
//    ListCreator<uint32_t> list{1, 2, 3};
//
//    ASSERT_EQ(&list.creatorItems[0], list.PeekFront());
//    ASSERT_EQ(&list.creatorItems[2], list.PeekBack());
//
//    list.Remove(list.creatorItems[1]);
//
//    EXPECT_EQ(&list.creatorItems[0], list.PeekFront());
//    EXPECT_EQ(&list.creatorItems[2], list.PeekBack());
//}
//
//TEST(TestList_PopBack, PopBack)
//{
//}
//
//TEST(TestList_PopFront, PopFront)
//{
//}
//
//TEST(TestList_PeekBack, PeekBack)
//{
//}
//
//TEST(TestList_PeekFront, PeekFront)
//{
//}