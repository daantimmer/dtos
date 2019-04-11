#pragma once

#include "DoubleLinkedList.hpp"
#include "os/criticalsection.hpp"

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
        ScopedCriticalSection critical;
        
		if (doubleLinkedList.Contains(item) == true)
		{
            return;
		}

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
        ScopedCriticalSection critical;

        return doubleLinkedList.PeekFront();
    }

    void PopFront()
    {
        ScopedCriticalSection critical;

        doubleLinkedList.PopFront();
    }

    auto Empty() const
    {
        ScopedCriticalSection critical;

        return doubleLinkedList.Empty();
    }

protected:
    DoubleLinkedList<T> doubleLinkedList;
    P p;
};
