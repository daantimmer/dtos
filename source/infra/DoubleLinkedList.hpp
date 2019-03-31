
#pragma once

#include "SingleLinkedList.hpp"

template<typename T>
struct DoubleLinkedList
{
    struct Item final
    {
        friend DoubleLinkedList;

        Item(T& obj)
            : object(obj)
        {
        }

        Item(T* obj)
            : object(*obj)
        {
            assert(obj != nullptr);
        }

    private:
        Item* next = nullptr;
        Item* previous = nullptr;
        T& object;
    };

    virtual std::size_t Size()
    {
        return count;
    }

    virtual bool Empty()
    {
        return Size() == 0;
    }

    virtual void AddBack(Item& item)
    {
        if (Empty() == true)
        {
            back = &item;
            front = &item;
        }
        else
        {
            item.next = back;

            back = &item;
            back->next->previous = back;
        }

        count++;
    }

    virtual void AddFront(Item& item)
    {
        if (Empty() == true)
        {
            back = &item;
            front = &item;
        }
        else
        {
            item.previous = front;

            front = &item;
            front->previous->next = front;
        }

        count++;
    }

    virtual void Remove(Item& item)
    {
        if (Empty() == true)
        {
            return;
        }

        if (&item == back)
        {
            PopBack();
            return;
        }
        else if (&item == front)
        {
            PopFront();
            return;
        }

        Item* iter = back;
        while (iter != nullptr)
        {
            if (iter == &item)
            {
                Item* previous = item.previous;
                Item* next = item.next;

                previous->next = next;
                next->previous = previous;

                item->next = nullptr;
                item->previous = nullptr;
                return;
            }
        }
    }

    virtual void PopBack()
    {
        if (Empty() == false)
        {
            back = back->next;
            back->previous = nullptr;
        }
    }

    virtual void PopFront()
    {
        if (Empty() == false)
        {
            front = front->previous;
            front->next = nullptr;
        }
    }

    virtual const T* PeekBack() const
    {
        if (Empty() == true)
        {
            return nullptr;
        }

        return &back->object;
    }

    virtual const T* PeekFront() const
    {
        if (Empty() == true)
        {
            return nullptr;
        }

        return &front->object;
    }

protected:
    Item* back = nullptr;
    Item* front = nullptr;

    std::size_t count = 0;
};
