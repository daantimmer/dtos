#include "infra/util/ext/SortedIntrusiveList.hpp"
#include "gmock/gmock.h"

struct ListItem: infra::SortedIntrusiveList<ListItem>::NodeType
{
    friend bool operator<(const ListItem&, const ListItem&);
    friend bool operator!=(const ListItem&, const ListItem&);

    explicit ListItem(int number)
        : number{number}
    {}

private:
    int number;
};

bool operator<(const ListItem& lhs, const ListItem& rhs)
{
    return lhs.number < rhs.number;
}

bool operator!=(const ListItem& lhs, const ListItem& rhs)
{
    return lhs.number != rhs.number;
}

bool operator>(const ListItem& lhs, const ListItem& rhs)
{
    return !(lhs < rhs) && (lhs != rhs);
}

bool operator==(const ListItem& lhs, const ListItem& rhs)
{
    return !(lhs < rhs) && !(lhs > rhs);
}

TEST(TestSortedIntrusiveList, InsertionDirectlySorts)
{
    auto sortedList = infra::SortedIntrusiveList<ListItem, std::greater<ListItem>>();

    ListItem l5{5};
    ListItem l2{2};
    ListItem l6{6};

    sortedList.push(l5);
    sortedList.push(l2);
    sortedList.push(l6);

    auto elem = std::begin(sortedList);
    EXPECT_THAT(*elem, ::testing::Ref(l2));

    elem = std::next(elem);
    EXPECT_THAT(*elem, ::testing::Ref(l5));

    elem = std::next(elem);
    EXPECT_THAT(*elem, ::testing::Ref(l6));
}

TEST(TestSortedIntrusiveList, PoppingRemovesFront)
{
    auto sortedList = infra::SortedIntrusiveList<ListItem, std::greater<ListItem>>();

    ListItem l6{6};
    ListItem l3{3};
    ListItem l5{5};

    ASSERT_THAT(sortedList.empty(), ::testing::IsTrue());

    sortedList.push(l6);
    sortedList.push(l3);
    sortedList.push(l5);

    ASSERT_THAT(sortedList.empty(), ::testing::IsFalse());

    EXPECT_THAT(sortedList.top(), ::testing::Ref(l3));

    sortedList.pop();
    EXPECT_THAT(sortedList.top(), ::testing::Ref(l5));

    sortedList.pop();
    EXPECT_THAT(sortedList.top(), ::testing::Ref(l6));

    sortedList.pop();
    EXPECT_THAT(sortedList.empty(), ::testing::IsTrue());
}

TEST(TestSortedIntrusiveList, SortingByGreaterAddsSameValuesNearTheEnd)
{
    auto sortedList = infra::SortedIntrusiveList<ListItem, std::greater<ListItem>>();

    ListItem l42{4};
    ListItem l41{4};
    ListItem l3{3};

    ASSERT_THAT(sortedList.empty(), ::testing::IsTrue());

    sortedList.push(l3);
    sortedList.push(l41);
    sortedList.push(l42);

    ASSERT_THAT(sortedList.empty(), ::testing::IsFalse());

    EXPECT_THAT(sortedList.top(), ::testing::Ref(l3));

    sortedList.pop();
    EXPECT_THAT(sortedList.top(), ::testing::Ref(l41));

    sortedList.pop();
    EXPECT_THAT(sortedList.top(), ::testing::Ref(l42));

    sortedList.pop();
    EXPECT_THAT(sortedList.empty(), ::testing::IsTrue());
}