
#pragma once

#include <cstdlib>

template<typename T>
struct SingleLinkedList
{
    struct Item final
    {
        friend SingleLinkedList;

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
        T& object;
    };

    std::size_t Size()
    {
        return count;
    }

    bool Empty()
    {
        return Size() == 0;
    }

    void AddBack(Item& item)
    {
        if (Empty() == true)
        {
            back = &item;
        }
        else
        {
            item.next = back;
            back = &item;
        }

        count++;
    }

    void Remove(Item& item)
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
        
        Item* iter = back;
        while (iter != nullptr)
        {
            if (iter->next == &item)
            {
                iter->next = item.next;
                item.next = nullptr;
                return;
            }
        }
    }

    void PopBack()
    {
        if (Empty() == false)
        {
            back = back->next;
        }
    }

    const T* PeekBack() const
    {
        if (Empty() == true)
        {
            return nullptr;
        }

        return &back->object;
    }

protected:
    Item* back = nullptr;

    std::size_t count = 0;
};