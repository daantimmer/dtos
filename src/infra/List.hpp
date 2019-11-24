
#pragma once

#include "kernel/criticalsection.hpp"

#include <cassert>
#include <cstddef>

template<typename T>
struct List
{
    struct Item final
    {
        friend List<T>;

        Item(T& obj)
            : object(obj)
        {
        }

        Item(T* obj)
            : object(*obj)
        {
            assert(obj != nullptr);
        }

        operator T*() const
        {
            return &object;
        }

        T& operator*()
        {
            return object;
        }

        T* operator->()
        {
            return &object;
        }

        Item* GetNext() const
        {
            return next;
        }

        Item* GetPrevious() const
        {
            return previous;
        }

        List* GetList() const
        {
            return list;
        }

        bool IsInAList() const
        {
            return list != nullptr;
        }

        void RemoveFromList()
        {
            if (IsInAList() == true)
            {
                list->Remove(*this);
            }
        }

    protected:
        Item* next = nullptr;
        Item* previous = nullptr;

        List* list = nullptr;

        T& object;
    };

    std::size_t Size() const
    {
        ScopedCriticalSection critical;

        return count;
    }

    bool Empty() const
    {
        ScopedCriticalSection critical;

        return Size() == 0;
    }

    bool Contains(const Item& item)
    {
        ScopedCriticalSection critical;

        return item.list == this;
    }

    void AddBack(Item& item)
    {
        ScopedCriticalSection critical;

        if (Contains(item) == true)
        {
            return;
        }
        else if (Empty() == true)
        {
            InsertFirst(item);
        }
        else
        {
            InsertBehind(*back, item);
        }
    }

    void AddFront(Item& item)
    {
        ScopedCriticalSection critical;

        if (Contains(item) == true)
        {
            return;
        }
        else if (Empty() == true)
        {
            InsertFirst(item);
        }
        else
        {
            InsertInfront(*front, item);
        }
    }

    void InsertInfront(Item& previous, Item& item)
    {
        ScopedCriticalSection critical;

        if (Contains(item) == true)
        {
            return;
        }

        auto next = previous.next;

        previous.next = &item;
        item.previous = &previous;

        item.next = next;

        if (next != nullptr)
        {
            next->previous = &item;
        }

        if (front == &previous)
        {
            front = &item;
        }

        item.list = this;
        count++;
    }

    void InsertBehind(Item& next, Item& item)
    {
        ScopedCriticalSection critical;

        if (Contains(item) == true)
        {
            return;
        }

        auto previous = next.previous;

        next.previous = &item;
        item.next = &next;

        item.previous = previous;

        if (previous != nullptr)
        {
            previous->next = &item;
        }

        if (back == &next)
        {
            back = &item;
        }

        item.list = this;
        count++;
    }

    void Remove(Item& item)
    {
        ScopedCriticalSection critical;

        if ((Empty() == true) || (Contains(item) == false))
        {
            /* return */
        }
        else if (&item == back)
        {
            PopBack();
        }
        else if (&item == front)
        {
            PopFront();
        }
        else
        {
            RemoveWithin(item);
        }
    }

    void PopBack()
    {
        ScopedCriticalSection critical;

        if (Empty() == false)
        {
            auto toBePopped = back;

            if (back == front)
            {
                back = nullptr;
                front = nullptr;
            }
            else
            {
                back = back->next;
                back->previous = nullptr;
            }

            ResetItem(*toBePopped);

            count--;
        }
    }

    void PopFront()
    {
        ScopedCriticalSection critical;

        if (Empty() == false)
        {
            auto toBePopped = front;

            if (back == front)
            {
                back = nullptr;
                front = nullptr;
            }
            else
            {
                front = front->previous;
                front->next = nullptr;
            }

            ResetItem(*toBePopped);

            count--;
        }
    }

    Item* PeekBack() const
    {
        ScopedCriticalSection critical;

        if (Empty() == true)
        {
            return nullptr;
        }

        return back;
    }

    Item* PeekFront() const
    {
        ScopedCriticalSection critical;

        if (Empty() == true)
        {
            return nullptr;
        }

        return front;
    }

protected:
    Item* back = nullptr;
    Item* front = nullptr;

    std::size_t count = 0;

    void ResetItem(Item& item)
    {
        item.next = nullptr;
        item.previous = nullptr;
        item.list = nullptr;
    }

    void InsertFirst(Item& item)
    {
        back = &item;
        front = &item;

        item.list = this;
        count = 1;
    }

    void RemoveWithin(Item& item)
    {
        Item* iter = back;
        while (iter != nullptr)
        {
            if (iter == &item)
            {
                Item* previous = item.previous;
                Item* next = item.next;

                previous->next = next;
                next->previous = previous;

                ResetItem(item);

                count--;

                return;
            }

            iter = iter->next;
        }
    }
};
