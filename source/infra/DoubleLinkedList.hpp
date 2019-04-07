
#pragma once

#include "SingleLinkedList.hpp"

#include <cassert>

template<typename T>
struct DoubleLinkedList
{
    struct Item final
    {
        friend DoubleLinkedList<T>;

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

        Item* GetNext() const
        {
            return next;
        }

        Item* GetPrevious() const
        {
            return previous;
        }

        DoubleLinkedList* GetList() const
        {
            return list;
        }

    protected:
        Item* next = nullptr;
        Item* previous = nullptr;

        DoubleLinkedList* list = nullptr;

        T& object;
    };

    virtual std::size_t Size() const
    {
        return count;
    }

    virtual bool Empty() const
    {
        return Size() == 0;
    }

    virtual void AddBack(Item& item)
    {
        if (Empty() == true)
        {
            InsertFirst(item);
        }
        else
        {
            InsertBehind(*back, item);
        }
    }

    virtual void AddFront(Item& item)
    {
        if (Empty() == true)
        {
            InsertFirst(item);
        }
        else
        {
            InsertInfront(*front, item);
        }
    }

    virtual void InsertInfront(Item& previous, Item& item)
    {
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

    virtual void InsertBehind(Item& next, Item& item)
    {
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

    virtual void Remove(Item& item)
    {
        if (Empty() == true)
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

    virtual void PopBack()
    {
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

    virtual void PopFront()
    {
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

    virtual Item* PeekBack() const
    {
        if (Empty() == true)
        {
            return nullptr;
        }

        return back;
    }

    virtual Item* PeekFront() const
    {
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
