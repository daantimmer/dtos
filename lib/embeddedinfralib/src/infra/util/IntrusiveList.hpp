#ifndef INFRA_INTRUSIVE_LIST_HPP
#define INFRA_INTRUSIVE_LIST_HPP

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <type_traits>

namespace infra
{
    template <class T>
    class IntrusiveList;

    namespace detail
    {
        template <class T>
        class IntrusiveListIterator;

        template <class T>
        struct IntrusiveListNode
        {
            // protected:
            constexpr IntrusiveListNode() = default;
            IntrusiveListNode(IntrusiveListNode&& other)
            {
                if (other.IsLinked() == false)
                {
                    Reset();
                    return;
                }

                next = other.next;
                previous = other.previous;

                next->previous = this;
                previous->next = this;

                other.Reset();
            }

            // public:
            IntrusiveListNode(const IntrusiveListNode& other) = delete;
            IntrusiveListNode& operator=(const IntrusiveListNode& other) = delete;
            IntrusiveListNode& operator=(IntrusiveListNode&& other) = delete;

            ~IntrusiveListNode();

            IntrusiveListNode& Next() const;
            IntrusiveListNode& Prev() const;

            bool IsLinked() const;
            void Link(IntrusiveListNode& position);
            void Unlink();

        private:
            template <class>
            friend class infra::detail::IntrusiveListIterator;

            // template <class>
            // friend class infra::IntrusiveList;

            void Reset();

            IntrusiveListNode* next{this};
            IntrusiveListNode* previous{this};
        };
    }

    template <class T>
    class IntrusiveList
    {
    public:
        typedef detail::IntrusiveListNode<T> NodeType;

        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef detail::IntrusiveListIterator<T> iterator;
        typedef detail::IntrusiveListIterator<const T> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename std::iterator_traits<iterator>::difference_type difference_type;
        typedef std::size_t size_type;

    public:
        IntrusiveList();

        template <class InputIterator>
        IntrusiveList(InputIterator first, InputIterator last);

        IntrusiveList(const IntrusiveList&) = delete;
        IntrusiveList(IntrusiveList&& other) = default;
        IntrusiveList& operator=(const IntrusiveList&) = delete;
        IntrusiveList& operator=(IntrusiveList&& other) = default;
        ~IntrusiveList();

    public:
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;

    public:
        size_type size() const;
        bool empty() const;
        bool has_element(const_reference value) const; // Runs in O(n) time

    public:
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

    public:
        void push_front(reference value);
        void push_back(reference value);
        void pop_front();
        void pop_back();

        void insert(iterator position, reference value);

        static void erase(reference value);
        static iterator erase(iterator position);
        static iterator erase(const_iterator position);

        static void transfer(reference value);

        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);

        void clear();

    public:
        bool operator==(const IntrusiveList& other) const;
        bool operator!=(const IntrusiveList& other) const;
        bool operator<(const IntrusiveList& other) const;
        bool operator<=(const IntrusiveList& other) const;
        bool operator>(const IntrusiveList& other) const;
        bool operator>=(const IntrusiveList& other) const;

    private:
        // size_type numberOfElements;
        // NodeType* root;
        NodeType root;
    };

    namespace detail
    {
        template <class T>
        class IntrusiveListIterator
        {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T* pointer;
            typedef T& reference;

        public:
            IntrusiveListIterator();
            explicit IntrusiveListIterator(const IntrusiveListNode<typename std::remove_const<T>::type>* node);
            template <class T2> // TICS !INT#001
            IntrusiveListIterator(const IntrusiveListIterator<T2>& other);

            template <class T2>
            IntrusiveListIterator& operator=(const IntrusiveListIterator<T2>& other);

            T& operator*() const;
            T* operator->() const;

            IntrusiveListIterator& operator++();
            IntrusiveListIterator operator++(int);
            IntrusiveListIterator& operator--();
            IntrusiveListIterator operator--(int);

            template <class T2>
            bool operator==(const IntrusiveListIterator<T2>& other) const;
            template <class T2>
            bool operator!=(const IntrusiveListIterator<T2>& other) const;

        private:
            template <class>
            friend class infra::detail::IntrusiveListIterator;
            template <class>
            friend class infra::IntrusiveList;

            const IntrusiveListNode<typename std::remove_const<T>::type>* node;
        };
    }

    ////    Implementation    ////

    template <class T>
    IntrusiveList<T>::IntrusiveList()
    // : numberOfElements(0)
    // : root(&root)
    {}

    template <class T>
    template <class InputIterator>
    IntrusiveList<T>::IntrusiveList(InputIterator first, InputIterator last)
    // : numberOfElements(0)
    // , root(&root)
    {
        assign(first, last);
    }

    // template <class T>
    // IntrusiveList<T>::IntrusiveList(IntrusiveList&& other)
    // // : numberOfElements(0)
    // // , root(&root)
    // {
    //     *this = std::move(other);
    // }

    // template <class T>
    // IntrusiveList<T>& IntrusiveList<T>::operator=(IntrusiveList&& /*other*/)
    // {
    //     // TODO
    //     // clear();
    //     // numberOfElements = other.numberOfElements;
    //     // if (other.root != &other.root)
    //     //     root = other.root;
    //     // root.previous = other.root.previous;
    //     // if (root.previous != nullptr)
    //     //     root.previous->next = &root;
    //     // other.root = &other.root;
    //     // other.root.previous = nullptr;
    //     // other.numberOfElements = 0;

    //     return *this;
    // }

    template <class T>
    IntrusiveList<T>::~IntrusiveList()
    {
        clear();
    }

    template <class T>
    typename IntrusiveList<T>::iterator IntrusiveList<T>::begin()
    {
        return iterator(&root.Next());
    }

    template <class T>
    typename IntrusiveList<T>::const_iterator IntrusiveList<T>::begin() const
    {
        return const_iterator(&root.Next());
    }

    template <class T>
    typename IntrusiveList<T>::iterator IntrusiveList<T>::end()
    {
        return iterator(&root);
    }

    template <class T>
    typename IntrusiveList<T>::const_iterator IntrusiveList<T>::end() const
    {
        return const_iterator(&root);
    }

    template <class T>
    typename IntrusiveList<T>::reverse_iterator IntrusiveList<T>::rbegin()
    {
        return reverse_iterator(end());
    }

    template <class T>
    typename IntrusiveList<T>::const_reverse_iterator IntrusiveList<T>::rbegin() const
    {
        return const_reverse_iterator(end());
    }

    template <class T>
    typename IntrusiveList<T>::reverse_iterator IntrusiveList<T>::rend()
    {
        return reverse_iterator(begin());
    }

    template <class T>
    typename IntrusiveList<T>::const_reverse_iterator IntrusiveList<T>::rend() const
    {
        return const_reverse_iterator(begin());
    }

    template <class T>
    typename IntrusiveList<T>::const_iterator IntrusiveList<T>::cbegin() const
    {
        return begin();
    }

    template <class T>
    typename IntrusiveList<T>::const_iterator IntrusiveList<T>::cend() const
    {
        return end();
    }

    template <class T>
    typename IntrusiveList<T>::const_reverse_iterator IntrusiveList<T>::crbegin() const
    {
        return rbegin();
    }

    template <class T>
    typename IntrusiveList<T>::const_reverse_iterator IntrusiveList<T>::crend() const
    {
        return rend();
    }

    template <class T>
    typename IntrusiveList<T>::size_type IntrusiveList<T>::size() const
    {
        return std::distance(begin(), end());
    }

    template <class T>
    bool IntrusiveList<T>::empty() const
    {
        return begin() == end();
    }

    template <class T>
    bool IntrusiveList<T>::has_element(const_reference value) const
    {
        for (const_reference item : *this)
            if (&item == &value)
                return true;

        return false;
    }

    template <class T>
    typename IntrusiveList<T>::reference IntrusiveList<T>::front()
    {
        return *begin();
    }

    template <class T>
    typename IntrusiveList<T>::const_reference IntrusiveList<T>::front() const
    {
        return *begin();
    }

    template <class T>
    typename IntrusiveList<T>::reference IntrusiveList<T>::back()
    {
        return *std::prev(end());
    }

    template <class T>
    typename IntrusiveList<T>::const_reference IntrusiveList<T>::back() const
    {
        return *std::prev(end());
    }

    template <class T>
    void IntrusiveList<T>::push_front(reference value)
    {
        insert(begin(), value);
    }

    template <class T>
    void IntrusiveList<T>::push_back(reference value)
    {
        insert(end(), value);
    }

    template <class T>
    void IntrusiveList<T>::pop_front()
    {
        erase(begin());
    }

    template <class T>
    void IntrusiveList<T>::pop_back()
    {
        erase(back());
    }

    template <class T>
    void IntrusiveList<T>::insert(iterator position, reference value)
    {
        value.Link(*position);
    }

    template <class T>
    void IntrusiveList<T>::erase(reference value)
    {
        value.Unlink();
    }

    template <class T>
    auto IntrusiveList<T>::erase(iterator position) -> iterator
    {
        auto next = std::next(position);

        erase(*position);

        return next;
    }

    template <class T>
    auto IntrusiveList<T>::erase(const_iterator position) -> iterator
    {
        auto next = std::next(position);

        erase(*position);

        return next;
    }

    template <class T>
    template <class InputIterator>
    void IntrusiveList<T>::assign(InputIterator first, InputIterator last)
    {
        clear();
        for (; first != last; ++first)
            push_back(*first);
    }

    template <class T>
    void IntrusiveList<T>::clear()
    {
        while (!empty())
            pop_front();
    }

    template <class T>
    bool IntrusiveList<T>::operator==(const IntrusiveList& other) const
    {
        return size() == other.size() && std::equal(begin(), end(), other.begin());
    }

    template <class T>
    bool IntrusiveList<T>::operator!=(const IntrusiveList& other) const
    {
        return !(*this == other);
    }

    template <class T>
    bool IntrusiveList<T>::operator<(const IntrusiveList& other) const
    {
        return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
    }

    template <class T>
    bool IntrusiveList<T>::operator<=(const IntrusiveList& other) const
    {
        return !(other < *this);
    }

    template <class T>
    bool IntrusiveList<T>::operator>(const IntrusiveList& other) const
    {
        return other < *this;
    }

    template <class T>
    bool IntrusiveList<T>::operator>=(const IntrusiveList& other) const
    {
        return !(*this < other);
    }

    namespace detail
    {
        // template <class T>
        // IntrusiveListNode<T>::IntrusiveListNode(const IntrusiveListNode&)
        // {}

        // template <class T>
        // IntrusiveListNode<T>& IntrusiveListNode<T>::operator=(const IntrusiveListNode& other)
        // {
        //     return *this;
        // }

        template <class T>
        IntrusiveListNode<T>::~IntrusiveListNode()
        {
            Unlink();
        }

        template <class T>
        IntrusiveListNode<T>& IntrusiveListNode<T>::Next() const
        {
            return *next;
        }

        template <class T>
        IntrusiveListNode<T>& IntrusiveListNode<T>::Prev() const
        {
            return *previous;
        }

        template <class T>
        bool IntrusiveListNode<T>::IsLinked() const
        {
            return next != this;
        }

        template <class T>
        void IntrusiveListNode<T>::Link(IntrusiveListNode& position)
        {
            Unlink();

            next = &position;
            previous = position.previous;

            position.previous->next = this;
            position.previous = this;
        }

        template <class T>
        void IntrusiveListNode<T>::Unlink()
        {
            previous->next = next;
            next->previous = previous;

            Reset();
        }

        template <class T>
        void IntrusiveListNode<T>::Reset()
        {
            next = this;
            previous = this;
        }

        template <class T>
        IntrusiveListIterator<T>::IntrusiveListIterator()
            : node(nullptr)
        {}

        template <class T>
        IntrusiveListIterator<T>::IntrusiveListIterator(
            const IntrusiveListNode<typename std::remove_const<T>::type>* node)
            : node(node)
        {}

        template <class T>
        template <class T2>
        IntrusiveListIterator<T>::IntrusiveListIterator(const IntrusiveListIterator<T2>& other)
            : node(other.node)
        {}

        template <class T>
        template <class T2>
        IntrusiveListIterator<T>& IntrusiveListIterator<T>::operator=(const IntrusiveListIterator<T2>& other)
        {
            node = other.node;

            return *this;
        }

        template <class T>
        T& IntrusiveListIterator<T>::operator*() const
        {
            return const_cast<T&>(static_cast<const T&>(*node));
        }

        template <class T>
        T* IntrusiveListIterator<T>::operator->() const
        {
            return const_cast<T*>(static_cast<const T*>(node));
        }

        template <class T>
        IntrusiveListIterator<T>& IntrusiveListIterator<T>::operator++()
        {
            node = node->next;
            return *this;
        }

        template <class T>
        IntrusiveListIterator<T> IntrusiveListIterator<T>::operator++(int)
        {
            IntrusiveListIterator copy(*this);
            node = node->next;
            return copy;
        }

        template <class T>
        IntrusiveListIterator<T>& IntrusiveListIterator<T>::operator--()
        {
            node = node->previous;
            return *this;
        }

        template <class T>
        IntrusiveListIterator<T> IntrusiveListIterator<T>::operator--(int)
        {
            IntrusiveListIterator copy(*this);
            node = node->previous;
            return copy;
        }

        template <class T>
        template <class T2>
        bool IntrusiveListIterator<T>::operator==(const IntrusiveListIterator<T2>& other) const
        {
            return node == other.node;
        }

        template <class T>
        template <class T2>
        bool IntrusiveListIterator<T>::operator!=(const IntrusiveListIterator<T2>& other) const
        {
            return !(*this == other);
        }
    }
}

#endif
