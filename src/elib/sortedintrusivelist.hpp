#pragma once

#include "elib/intrusivelist.hpp"

#include <algorithm>

namespace elib
{
template<class Compare, class BaseClass, IntrusiveListNode BaseClass::*Node, class DerivedClass = BaseClass>
struct SortedIntrusiveList
{
    using UnsortedIntrusiveList = IntrusiveList<BaseClass, Node, DerivedClass>;

    using value_type = typename UnsortedIntrusiveList::value_type;
    using pointer = typename UnsortedIntrusiveList::pointer;
    using reference = typename UnsortedIntrusiveList::reference;
    using const_pointer = typename UnsortedIntrusiveList::const_pointer;
    using const_reference = typename UnsortedIntrusiveList::const_reference;
    using iterator = typename UnsortedIntrusiveList::iterator;
    using reverse_iterator = typename UnsortedIntrusiveList::reverse_iterator;
    using const_iterator = typename UnsortedIntrusiveList::const_iterator;
    using const_reverse_iterator = typename UnsortedIntrusiveList::const_reverse_iterator;

    constexpr explicit SortedIntrusiveList(const Compare& compare = Compare{})
        : implementation{compare}
    {
    }

    bool empty() const
    {
        return implementation.base.empty();
    }

    void clear()
    {
        implementation.base.clear();
    }

    reference back()
    {
        return implementation.base.back();
    }

    const_reference back() const
    {
        return implementation.base.back();
    }

    reference front()
    {
        return implementation.base.front();
    }

    const_reference front() const
    {
        return implementation.base.front();
    }

    iterator begin()
    {
        return implementation.base.begin();
    }

    reverse_iterator rbegin()
    {
        return implementation.base.rbegin();
    }

    const_iterator begin() const
    {
        return implementation.base.begin();
    }

    const_reverse_iterator rbegin() const
    {
        return implementation.base.rbegin();
    }

    const_iterator cbegin() const
    {
        return implementation.base.cbegin();
    }

    const_reverse_iterator crbegin() const
    {
        return implementation.base.crbegin();
    }

    iterator end()
    {
        return implementation.base.end();
    }

    reverse_iterator rend()
    {
        return implementation.base.rend();
    }

    const_iterator end() const
    {
        return implementation.base.end();
    }

    const_reverse_iterator rend() const
    {
        return implementation.base.rend();
    }

    const_iterator cend() const
    {
        return implementation.base.cend();
    }

    const_reverse_iterator crend() const
    {
        return implementation.base.crend();
    }

    void pop_front()
    {
        implementation.base.pop_front();
    }

    void pop_back()
    {
        implementation.base.pop_back();
    }

    iterator insert(reference element)
    {
        return UnsortedIntrusiveList::insert(implementation.findInsertPosition(element), element);
    }

    void splice(const_iterator element)
    {
        UnsortedIntrusiveList::splice(implementation.findInsertPosition(*element), element);
    }

    void swap(SortedIntrusiveList& other)
    {
        implementation.swap(other.implementation);
    }

    static iterator erase(const_iterator position)
    {
        return UnsortedIntrusiveList::erase(position);
    }

    SortedIntrusiveList(const SortedIntrusiveList&) = delete;
    SortedIntrusiveList(SortedIntrusiveList&&) = default;
    const SortedIntrusiveList& operator=(const SortedIntrusiveList&) = delete;
    SortedIntrusiveList operator=(SortedIntrusiveList&&) = delete;

    struct Implementation
    {
        constexpr explicit Implementation(const Compare& compare)
            : comparer{compare}
            , base{}
        {
        }

        iterator findInsertPosition(const_reference newElement)
        {
            return std::find_if(std::begin(base), std::end(base), [this, &newElement](const auto& element) {
                return this->comparer(element, newElement);
            });
        }

        void swap(Implementation& other)
        {
            base.swap(other.base);
            std::swap(this->comparer, other.comparer);
        }

        Implementation(const Implementation&) = delete;
        Implementation(Implementation&&) = default;
        const Implementation& operator=(const Implementation&) = delete;
        Implementation& operator=(Implementation&&) = delete;

        Compare comparer;
        UnsortedIntrusiveList base;
    };

    Implementation implementation;
};
}