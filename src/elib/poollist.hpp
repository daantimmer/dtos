#pragma once

#include "mempool.hpp"

#include <algorithm>
#include <iterator>
#include <new>
#include <type_traits>

namespace elib
{
    template<typename T>
    struct parameter_type
    {
        using type =
        typename std::conditional<std::is_fundamental<T>::value || std::is_pointer<T>::value, T, const T&>::type;
    };

    struct PoolListPrivate
    {
        using size_type = size_t;

        struct Node
        {
            void reverse()
            {
                std::swap(previous, next);
            }

            Node* previous = nullptr;
            Node* next = nullptr;
        };

    protected:
        explicit PoolListPrivate(elib::MempoolBase& nodePool)
            : pool(nodePool)
        {
            Join(startAndEnd, startAndEnd);
        }

        Node& Head()
        {
            return *startAndEnd.next;
        }

        Node& Head() const
        {
            return *startAndEnd.next;
        }

        Node& Tail()
        {
            return *startAndEnd.previous;
        }

        Node& Tail() const
        {
            return *startAndEnd.previous;
        }

        void InsertNode(Node& position, Node& newNode)
        {
            Join(*position.previous, newNode);
            Join(newNode, position);
        }

        void Join(Node& left, Node& right)
        {
            left.next = &right;
            right.previous = &left;
        }

        Node startAndEnd;

        elib::MempoolBase& pool;
    };

    template<typename T>
    struct PoolListBase : PoolListPrivate
    {
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using rvalue_reference = T&&;
        using size_type = std::size_t;

        struct iterator
        {
            friend PoolListBase;

            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            iterator() = default;
            explicit iterator(Node& node)
                : node(&node)
            {
            }

            iterator(const iterator& other)
                : node(other.node)
            {
            }

            iterator& operator++()
            {
                node = node->next;
                return *this;
            }

            iterator operator++(int)
            {
                iterator temp(*this);
                node = node->next;
                return temp;
            }

            iterator& operator--()
            {
                node = node->previous;
                return *this;
            }

            iterator operator--(int)
            {
                iterator temp(*this);
                node = node->previous;
                return temp;
            }

            iterator operator=(const iterator& other)
            {
                node = other.node;
                return *this;
            }

            reference operator*()
            {
                return DataCast(node)->value;
            }

            const_reference operator*() const
            {
                return DataCast(node)->value;
            }

            pointer operator&()
            {
                return &(DataCast(node)->value);
            }

            const_pointer operator&() const
            {
                return &(DataCast(node)->value);
            }

            pointer operator->()
            {
                return &(DataCast(node)->value);
            }

            const_pointer operator->() const
            {
                return &(DataCast(node)->value);
            }

            friend bool operator==(const iterator& lhs, const iterator& rhs)
            {
                return lhs.node == rhs.node;
            }

            friend bool operator!=(const iterator& lhs, const iterator& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            Node* node = nullptr;
        };

        struct const_iterator
        {
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = const T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            const_iterator() = default;

            explicit const_iterator(const Node& node)
                : node(&node)
            {
            }

            const_iterator(const const_iterator& other)
                : node(other.node)
            {
            }

            const_iterator(const iterator& other)
                : node(other.node)
            {
            }

            const_iterator& operator++()
            {
                node = node->next;
                return *this;
            }

            const_iterator operator++(int)
            {
                const_iterator temp(*this);
                node = node->next;
                return temp;
            }

            const_iterator& operator--()
            {
                node = node->previous;
                return *this;
            }

            const_iterator operator--(int)
            {
                const_iterator temp(*this);
                node = node->previous;
                return temp;
            }

            const_iterator operator=(const const_iterator& other)
            {
                node = other.node;
                return *this;
            }

            const_reference operator*() const
            {
                return DataCast(node)->value;
            }

            const_pointer operator&() const
            {
                return &(DataCast(node)->value);
            }

            const_pointer operator->() const
            {
                return &(DataCast(node)->value);
            }

            friend bool operator==(const const_iterator& lhs, const const_iterator& rhs)
            {
                return lhs.node == rhs.node;
            }

            friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            const Node* node = nullptr;
        };

        using difference_type = typename iterator::difference_type;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        auto begin()
        {
            return iterator{Head()};
        }

        auto begin() const
        {
            return const_iterator{Head()};
        }

        auto end()
        {
            return iterator{startAndEnd};
        }

        auto end() const
        {
            return const_iterator{startAndEnd};
        }

        auto cbegin() const
        {
            return const_iterator{Head()};
        }

        auto cend() const
        {
            return const_iterator{startAndEnd};
        }

        auto rbegin()
        {
            return reverse_iterator{iterator(startAndEnd)};
        }

        auto rbegin() const
        {
            return const_reverse_iterator{const_iterator(startAndEnd)};
        }

        auto rend()
        {
            return reverse_iterator{iterator(Head())};
        }

        auto rend() const
        {
            return const_reverse_iterator{iterator(Head())};
        }

        auto crbegin() const
        {
            return const_reverse_iterator{const_iterator(startAndEnd)};
        }

        auto crend() const
        {
            return const_reverse_iterator{const_iterator(Head())};
        }

        reference front()
        {
            return DataCast(Head()).value;
        }

        const_reference front() const
        {
            return DataCast(Head()).value;
        }

        reference back()
        {
            return DataCast(Tail()).value;
        }

        const_reference back() const
        {
            return DataCast(Tail()).value;
        }

        template<typename TIterator>
        void assign(TIterator first, TIterator last)
        {
            Initialise();

            std::for_each(first, last, [this](auto item) {
                auto node = AllocateDataNode(item);
                Join(Tail(), node);
                Join(node, startAndEnd);
            });
        }

        void assign(std::size_t n, const_reference value)
        {
            Initialise();

            while (n-- > 0)
            {
                auto node = AllocateDataNode(value);
                Join(*startAndEnd.previous, node);
                Join(node, startAndEnd);
            }
        }

        void push_front(const_reference value)
        {
            InsertNode(Head(), AllocateDataNode(value));
        }

        void push_front(rvalue_reference value)
        {
            InsertNode(Head(), AllocateDataNode(std::move(value)));
        }

        template<typename... Args>
        void emplace_front(Args&&... args)
        {
            auto dataNodePtr = pool.Allocate<DataNode>();
            ::new (&(dataNodePtr->value)) T(std::forward<Args>(args)...);
            InsertNode(Head(), *dataNodePtr);
        }

        void pop_front()
        {
            RemoveNode(Head());
        }

        void push_back(const_reference value)
        {
            InsertNode(startAndEnd, AllocateDataNode(value));
        }

        void push_back(rvalue_reference value)
        {
            InsertNode(startAndEnd, AllocateDataNode(std::move(value)));
        }

        template<typename... Args>
        void emplace_back(Args&&... args)
        {
            auto dataNodePtr = pool.Allocate<DataNode>();
            ::new (&(dataNodePtr->value)) T(std::forward<Args>(args)...);
            InsertNode(startAndEnd, *dataNodePtr);
        }

        void pop_back()
        {
            RemoveNode(Tail());
        }

    protected:
        using parameter_t = typename parameter_type<T>::type;

        struct DataNode : Node
        {
            explicit DataNode(const T& value)
                : value(value)
            {
            }

            T value;
        };

        void Initialise()
        {
            pool.ReleaseAll();

            Join(startAndEnd, startAndEnd);
        }

    public:
        PoolListBase(elib::MempoolBase& pool) /* TODO move to protected */
            : PoolListPrivate(pool)
        {
        }

    private:
        static DataNode* DataCast(Node* node)
        {
            return reinterpret_cast<DataNode*>(node);
        }

        static DataNode& DataCast(Node& node)
        {
            return reinterpret_cast<DataNode&>(node);
        }

        static const DataNode* DataCast(const Node* node)
        {
            return reinterpret_cast<const DataNode*>(node);
        }

        static const DataNode& DataCast(const Node& node)
        {
            return reinterpret_cast<const DataNode&>(node);
        }

        void RemoveNode(Node& node)
        {
            Join(*node.previous, *node.next);

            DestroyDataNode(static_cast<DataNode&>(node));
        }

        DataNode& AllocateDataNode(const_reference value)
        {
            auto dataNodePtr = pool.Allocate<DataNode>();

            ::new (&(dataNodePtr->value)) T(value);

            return *dataNodePtr;
        }

        DataNode& AllocateDataNode(rvalue_reference value)
        {
            auto dataNodePtr = pool.Allocate<DataNode>();

            ::new (&(dataNodePtr->value)) T(std::move(value));

            return *dataNodePtr;
        }

        void DestroyDataNode(DataNode& node)
        {
            pool.Destroy(&node);
            //node.value.~T();
            //pool.Release(&node);
        }
    };

    template<typename T, const std::size_t MAX_SIZE_TPL>
    struct PoolList : PoolListBase<T>
    {
    protected:
        using base = PoolListBase<T>;

    public:
        static constexpr std::size_t MAX_SIZE = MAX_SIZE_TPL;

        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using rvalue_reference = T&&;
        using size_type = std::size_t;

        PoolList()
            : base(pool, MAX_SIZE, false)
        {
        }

        explicit PoolList(std::size_t initialSize, const_reference value)
            : base(pool /*, MAX_SIZE, false*/)
        {
            this->assign(initialSize, value);
        }

        PoolList(const PoolList& other)
            : base(pool /*, MAX_SIZE, false*/)
        {
            if (this != &other)
            {
                this->assign(other.cbegin(), other.cend());
            }
        }

        PoolList(PoolList&& other)
            : base(pool)
        {
            if (this != &other)
            {
                this->Initialise();

                auto itr = other.begin();
                while (itr != other.end())
                {
                    this->push_back(std::move(*itr));
                }

                other.Initialise();
            }
        }

        template<typename Iterator>
        PoolList(Iterator first, Iterator last)
            : base(pool)
        {
            this->assign(first, last);
        }

        PoolList(std::initializer_list<T> init)
            : base(pool)
        {
            this->assign(init.begin(), init.end());
        }

        PoolList& operator=(const PoolList& rhs)
        {
            if (&rhs != this)
            {
                this->assign(rhs.cbegin(), rhs.cend());
            }

            return *this;
        }

        PoolList operator=(PoolList&& rhs)
        {
            if (&rhs != this)
            {
                this->Initialise();

                auto itr = rhs.begin();
                while (itr != rhs.end())
                {
                    this->push_back(std::move(*itr));
                }

                rhs.Initialise();
            }

            return *this;
        }

    private:
        elib::Mempool<typename base::DataNode, MAX_SIZE> pool;
    };

    template<typename T>
    bool operator==(const PoolListBase<T>& lhs, const PoolListBase<T>& rhs)
    {
        return (lhs.size() == rhs.size()) && std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
    }

	template<typename T>
	bool operator!=(const PoolListBase<T>& lhs, const PoolListBase<T>& rhs)
	{
        return !(lhs == rhs);
	}

    template<typename T>
    bool operator<(const PoolListBase<T>& lhs, const PoolListBase<T>& rhs)
	{
        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
	}
		
	template<typename T>
    bool operator>(const PoolListBase<T>& lhs, const PoolListBase<T>& rhs)
    {
        return (rhs < lhs);
    }	

	template<typename T>
    bool operator<=(const PoolListBase<T>& lhs, const PoolListBase<T>& rhs)
    {
        return !(lhs > rhs);
    }	

	template<typename T>
    bool operator>=(const PoolListBase<T>& lhs, const PoolListBase<T>& rhs)
    {
        return !(lhs < rhs);
    }

} // namespace elib