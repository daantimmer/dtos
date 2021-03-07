#pragma once

#include <cstddef>
#include <iterator>

namespace elib
{
    struct IntrusiveListNode
    {
        constexpr IntrusiveListNode() = default;

        IntrusiveListNode(IntrusiveListNode&& other)
        {
            if (other.IsLinked() == false)
            {
                Reset();
                return;
            }

            nextNode = other.nextNode;
            previousNode = other.previousNode;

            nextNode->previousNode = this;
            previousNode->nextNode = this;

            other.Reset();
        }

        ~IntrusiveListNode()
        {
            Unlink();
        }

        IntrusiveListNode& GetNextNode() const
        {
            return *nextNode;
        }

        IntrusiveListNode& GetPreviousNode() const
        {
            return *previousNode;
        }

        bool IsLinked() const
        {
            return nextNode != this;
        }

        void Link(IntrusiveListNode& position)
        {
            Unlink();

            nextNode = &position;
            previousNode = position.previousNode;

            position.previousNode->nextNode = this;
            position.previousNode = this;
        }

        void Swap()
        {}

        void Unlink()
        {
            previousNode->nextNode = nextNode;
            nextNode->previousNode = previousNode;

            Reset();
        }

        IntrusiveListNode(const IntrusiveListNode&) = delete;
        const IntrusiveListNode& operator=(const IntrusiveListNode&) = delete;
        IntrusiveListNode& operator=(IntrusiveListNode&&) = delete;

    private:
        void Reset()
        {
            nextNode = this;
            previousNode = this;
        }

        IntrusiveListNode* previousNode{this};
        IntrusiveListNode* nextNode{this};
    };

    struct IntrusiveListBase
    {
        constexpr IntrusiveListBase() = default;

        ~IntrusiveListBase()
        {
            clear();
        }

        bool empty() const
        {
            return &begin() == &end();
        }

        IntrusiveListNode& begin()
        {
            return rootNode.GetNextNode();
        }

        const IntrusiveListNode& begin() const
        {
            return rootNode.GetNextNode();
        }

        const IntrusiveListNode& cbegin() const
        {
            return begin();
        }

        IntrusiveListNode& end()
        {
            return rootNode;
        }

        const IntrusiveListNode& end() const
        {
            return rootNode;
        }

        const IntrusiveListNode& cend() const
        {
            return end();
        }

        void clear()
        {
            while (empty() == false)
            {
                pop_front();
            }
        }

        void pop_back()
        {
            erase(end().GetPreviousNode());
        }

        void pop_front()
        {
            erase(begin());
        }

        void push_back(IntrusiveListNode& node)
        {
            insert(end(), node);
        }

        void push_front(IntrusiveListNode& node)
        {
            insert(begin(), node);
        }

        void swap()
        {}

        static IntrusiveListNode& erase(IntrusiveListNode& position)
        {
            auto& next = position.GetNextNode();
            position.Unlink();
            return next;
        }

        static void insert(IntrusiveListNode& position, IntrusiveListNode& node)
        {
            node.Link(position);
        }

        static void splice(IntrusiveListNode& position, IntrusiveListNode& node)
        {
            insert(position, node);
        }

        IntrusiveListBase(const IntrusiveListBase&) = delete;
        IntrusiveListBase(IntrusiveListBase&&) = default;

        const IntrusiveListBase& operator=(const IntrusiveListBase&) = delete;
        IntrusiveListBase& operator=(IntrusiveListBase&&) = delete;

    private:
        IntrusiveListNode rootNode;
    };

    template <class BaseClass, IntrusiveListNode BaseClass::*Node, class DerivedClass = BaseClass>
    struct IntrusiveListIterator
    {
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = DerivedClass;
        using pointer = value_type*;
        using reference = value_type&;

        constexpr IntrusiveListIterator() = default;

        constexpr explicit IntrusiveListIterator(IntrusiveListNode* const node)
            : node(node)
        {}

        constexpr explicit IntrusiveListIterator(reference element)
            : node(&(element.*Node))
        {}

        pointer operator->() const
        {
            return getPointer();
        }

        reference operator*() const
        {
            return *getPointer();
        }

        IntrusiveListIterator& operator++()
        {
            node = &node->GetNextNode();
            return *this;
        }

        IntrusiveListIterator& operator--()
        {
            node = &node->GetPreviousNode();
            return *this;
        }

        bool operator==(const IntrusiveListIterator& other) const
        {
            return node == other.node;
        }

        bool operator!=(const IntrusiveListIterator& other) const
        {
            return !(*this == other);
        }

        pointer getPointer() const
        {
            const auto offset = reinterpret_cast<size_t>(&(static_cast<pointer>(nullptr)->*Node));
            return reinterpret_cast<pointer>(reinterpret_cast<size_t>(node) - offset);
        }

        IntrusiveListNode* node = nullptr;
    };

    template <class BaseClass, IntrusiveListNode BaseClass::*Node, class DerivedClass = BaseClass>
    struct IntrusiveListConstIterator
    {
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = DerivedClass;
        using pointer = value_type*;
        using reference = value_type&;

        constexpr IntrusiveListConstIterator()
            : node{}
        {}

        constexpr explicit IntrusiveListConstIterator(const IntrusiveListNode* const node)
            : node{node}
        {}

        constexpr explicit IntrusiveListConstIterator(reference element)
            : node{&(element.*Node)}
        {}

        template <IntrusiveListNode BaseClass::*NonConstNode>
        constexpr IntrusiveListConstIterator(
            const IntrusiveListIterator<BaseClass, NonConstNode, DerivedClass>& iterator)
            : IntrusiveListConstIterator{*iterator}
        {}

        pointer operator->() const
        {
            return getPointer();
        }

        reference operator*() const
        {
            return *getPointer();
        }

        IntrusiveListConstIterator& operator++()
        {
            node = &node->GetNextNode();
            return *this;
        }

        IntrusiveListConstIterator& operator--()
        {
            node = &node->GetPreviousNode();
            return *this;
        }

        bool operator==(const IntrusiveListConstIterator& other) const
        {
            return node == other.node;
        }

    private:
        pointer getPointer() const
        {
            const auto offset = reinterpret_cast<size_t>(&(static_cast<pointer>(nullptr)->*Node));
            return reinterpret_cast<pointer>(reinterpret_cast<size_t>(node) - offset);
        }

        const IntrusiveListNode* node;
    };

    template <class BaseClass, IntrusiveListNode BaseClass::*Node, class DerivedClass = BaseClass>
    struct IntrusiveList
    {
        using value_type = DerivedClass;

        using pointer = value_type*;
        using reference = value_type&;

        using const_pointer = const value_type*;
        using const_reference = const value_type&;

        using iterator = IntrusiveListIterator<BaseClass, Node, BaseClass>;
        using reverse_iterator = IntrusiveListIterator<BaseClass, Node, BaseClass>;

        using const_iterator = IntrusiveListConstIterator<BaseClass, Node, BaseClass>;
        using const_reverse_iterator = IntrusiveListIterator<BaseClass, Node, BaseClass>;

        constexpr IntrusiveList() = default;

        bool empty() const
        {
            return base.empty();
        }

        void clear()
        {
            base.clear();
        }

        reference back()
        {
            return *--end();
        }

        const_reference back() const
        {
            return *--end();
        }

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *begin();
        }

        iterator begin()
        {
            return iterator{&base.begin()};
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator{end()};
        }

        const_iterator begin() const
        {
            return const_iterator{&base.begin()};
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator{end()};
        }

        const_iterator cbegin() const
        {
            return const_iterator{begin()};
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator{rbegin()};
        }

        iterator end()
        {
            return iterator{&base.end()};
        }

        reverse_iterator rend()
        {
            return reverse_iterator{begin()};
        }

        const_iterator end() const
        {
            return const_iterator{&base.end()};
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator{begin()};
        }

        const_iterator cend() const
        {
            return const_iterator{end()};
        }

        const_reverse_iterator crend() const
        {
            return const_reverse_iterator{rend()};
        }

        void pop_front()
        {
            erase(begin());
        }

        void pop_back()
        {
            erase(--end());
        }

        void push_front(reference element)
        {
            insert(begin(), element);
        }

        void push_back(reference element)
        {
            insert(end(), element);
        }

        static iterator insert(const_iterator position, reference element)
        {
            auto& nodePosition = (*position).*Node;
            auto& node = element.*Node;

            IntrusiveListBase::insert(nodePosition, node);

            return iterator{&node};
        }

        static void splice(const_iterator position, const_iterator element)
        {
            auto& nodePosition = (*position).*Node;
            auto& node = (*element).*Node;

            IntrusiveListBase::splice(nodePosition, node);
        }

        static iterator erase(const_iterator position)
        {
            auto& nodePosition = (*position).*Node;
            auto& next = IntrusiveListBase::erase(nodePosition);
            return iterator{&next};
        }

        IntrusiveList(const IntrusiveList&) = delete;
        IntrusiveList(IntrusiveList&&) = default;
        const IntrusiveList& operator=(const IntrusiveList&) = delete;
        IntrusiveList& operator=(IntrusiveList&&) = delete;

    private:
        IntrusiveListBase base;
    };
}