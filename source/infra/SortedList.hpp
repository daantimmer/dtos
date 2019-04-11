#pragma once

#include "List.hpp"
#include "os/criticalsection.hpp"

template<typename T, typename P>
struct SortedList
{
    using TypeList = List<T>;
    using Item = typename TypeList::Item;

    SortedList(P p)
        : p(p)
    {
    }

    void Insert(Item& item)
    {
        ScopedCriticalSection critical;
        
		if (list.Contains(item) == true)
		{
            return;
		}

        Item* iterator = list.PeekFront();

        while (iterator != nullptr)
        {
            if (p(*iterator, item) == true)
            {
                list.InsertInfront(*iterator, item);
                return;
            }

            iterator = iterator->GetNext();
        }

        list.AddBack(item);
    }

    auto PeekFront()
    {
        ScopedCriticalSection critical;

        return list.PeekFront();
    }

    void PopFront()
    {
        ScopedCriticalSection critical;

        list.PopFront();
    }

    auto Empty() const
    {
        ScopedCriticalSection critical;

        return list.Empty();
    }

protected:
    List<T> list;
    P p;
};
