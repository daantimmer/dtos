#pragma once

#include "List.hpp"
#include "kernel/criticalsection.hpp"

template <typename T, typename P>
struct SortedList: protected List<T>
{
    using Item = typename List<T>::Item;

    SortedList(P p)
        : p(p)
    {}

    void Insert(Item& item)
    {
        ScopedCriticalSection critical;

        if (Contains(item) == true)
        {
            return;
        }

        Item* iterator = PeekFront();

        while (iterator != nullptr)
        {
            if (p(*iterator, item) == true)
            {
                this->InsertInfront(*iterator, item);
                return;
            }

            iterator = iterator->GetNext();
        }

        this->AddBack(item);
    }

    using List<T>::Remove;
    using List<T>::Contains;
    using List<T>::Empty;
    using List<T>::PeekFront;
    using List<T>::PopFront;

protected:
    P p;
};
