#pragma once

#include "DoubleLinkedList.hpp"

template<typename T, typename P>
struct SortedDoubleLinkedList
{
    using TypeList = DoubleLinkedList<T>;
    using Item = typename TypeList::Item;

    SortedDoubleLinkedList(P p)
        : p(p)
    {
    }

    void Insert(Item& item)
    {
        Item* location = nullptr;
        Item* iterator = doubleLinkedList.PeekFront();

        while (iterator != nullptr)
        {
            if (p(*iterator, item) == true)
            {
                doubleLinkedList.InsertInfront(*iterator, item);
                return;
            }

            iterator = iterator->GetNext();
        }

		doubleLinkedList.AddBack(item);
    }

	auto PeekFront()
	{
        return doubleLinkedList.PeekFront();
	}

	auto PopFront()
	{
        return doubleLinkedList.PopFront();
	}

protected:
    DoubleLinkedList<T> doubleLinkedList;
    P p;
};
