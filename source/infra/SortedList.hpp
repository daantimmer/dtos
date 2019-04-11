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
        
		if (List.Contains(item) == true)
		{
            return;
		}

        Item* iterator = List.PeekFront();

        while (iterator != nullptr)
        {
            if (p(*iterator, item) == true)
            {
                List.InsertInfront(*iterator, item);
                return;
            }

            iterator = iterator->GetNext();
        }

        List.AddBack(item);
    }

    auto PeekFront()
    {
        ScopedCriticalSection critical;

        return List.PeekFront();
    }

    void PopFront()
    {
        ScopedCriticalSection critical;

        List.PopFront();
    }

    auto Empty() const
    {
        ScopedCriticalSection critical;

        return List.Empty();
    }

protected:
    List<T> List;
    P p;
};
