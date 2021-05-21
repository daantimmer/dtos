#ifndef EXT_SORTEDINTRUSIVELIST_HPP
#define EXT_SORTEDINTRUSIVELIST_HPP

#include "infra/util/IntrusiveList.hpp"

namespace infra
{
    template <class T, class TCompare = std::less<T>>
    struct SortedIntrusiveList
    {
        using UnsortedIntrusiveList = IntrusiveList<T>;

        using value_type = typename UnsortedIntrusiveList::value_type;
        using reference = typename UnsortedIntrusiveList::reference;
        using const_reference = typename UnsortedIntrusiveList::const_reference;
        using pointer = typename UnsortedIntrusiveList::pointer;
        using const_pointer = typename UnsortedIntrusiveList::const_pointer;
        using iterator = typename UnsortedIntrusiveList::iterator;
        using const_iterator = typename UnsortedIntrusiveList::const_iterator;
        using reverse_iterator = typename UnsortedIntrusiveList::reverse_iterator;
        using const_reverse_iterator = typename UnsortedIntrusiveList::const_reverse_iterator;

        using difference_type = typename UnsortedIntrusiveList::difference_type;
        using size_type = typename UnsortedIntrusiveList::size_type;

        using NodeType = typename UnsortedIntrusiveList::NodeType;

        iterator begin()
        {
            return base.begin();
        }
        const_iterator begin() const
        {
            return base.begin();
        }
        iterator end()
        {
            return base.end();
        }
        const_iterator end() const
        {
            return base.end();
        }

        reverse_iterator rbegin()
        {
            return base.rbegin();
        }
        const_reverse_iterator rbegin() const
        {
            return base.rbegin();
        }
        reverse_iterator rend()
        {
            return base.rend();
        }
        const_reverse_iterator rend() const
        {
            return base.rend();
        }

        const_iterator cbegin() const
        {
            return base.cbegin();
        }
        const_iterator cend() const
        {
            return base.cend();
        }

        const_reverse_iterator crbegin() const
        {
            return base.crbegin();
        }
        const_reverse_iterator crend() const
        {
            return base.crend();
        }

        size_type size() const
        {
            return base.size();
        }
        bool empty() const
        {
            return base.empty();
        }
        bool has_element(const_reference value) const // Runs in O(n) time
        {
            return base.has_element(value);
        }

        reference front()
        {
            return base.front();
        }
        const_reference front() const
        {
            return base.front();
        }
        reference back()
        {
            return base.back();
        }
        const_reference back() const
        {
            return base.back();
        }

        reference top()
        {
            return base.front();
        }

        const_reference top() const
        {
            return base.front();
        }

        void push(reference value)
        {
            auto iter = std::find_if(std::begin(base), std::end(base), [/*this, */ &value](const auto& element) {
                return TCompare()(element, value);
            });
            base.insert(iter, value);
        }

        void pop()
        {
            base.pop_front();
        }

        static void erase(reference value)
        {
            UnsortedIntrusiveList::erase(value);
        }

        static iterator erase(iterator position)
        {
            return UnsortedIntrusiveList::erase(position);
        }

        static iterator erase(const_iterator position)
        {
            return UnsortedIntrusiveList::erase(position);
        }

        void transfer(reference value)
        {
            UnsortedIntrusiveList::erase(value);
            push(value);
        }

        void swap(SortedIntrusiveList& other)
        {
            base.swap(other);
        }

        void clear()
        {
            base.clear();
        }

        bool operator==(const SortedIntrusiveList& other) const
        {
            return base.operator==(other);
        }
        bool operator!=(const SortedIntrusiveList& other) const
        {
            return base.operator!=(other);
        }
        bool operator<(const SortedIntrusiveList& other) const
        {
            return base.operator<(other);
        }
        bool operator<=(const SortedIntrusiveList& other) const
        {
            return base.operator<=(other);
        }
        bool operator>(const SortedIntrusiveList& other) const
        {
            return base.operator>(other);
        }
        bool operator>=(const SortedIntrusiveList& other) const
        {
            return base.operator=>(other);
        }

    private:
        UnsortedIntrusiveList base;
    };
}

#endif
