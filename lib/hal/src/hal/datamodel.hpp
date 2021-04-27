#ifndef HAL_DATAMODEL_HPP
#define HAL_DATAMODEL_HPP

#include "infra/util/Function.hpp"
#include "infra/util/IntrusiveList.hpp"
#include <tuple>

namespace hal
{
    struct NoNotification
    {
        struct Callback;

        template <class T>
        bool IsEqual(const T&, const T&) const
        {
            return false;
        }

        void Notify() const
        {}
    };

    struct Notification
    {
        struct Callback: infra::IntrusiveList<Callback>::NodeType
        {
            Callback(infra::Function<void()> func)
                : func{func}
            {}

            infra::Function<void()> func;
        };

        template <class T>
        bool IsEqual(const T&, const T&) const;
        void Notify() const;

        infra::IntrusiveList<Callback> listeners;
    };

    template <class Tag, class TBase, class TNotification = NoNotification>
    struct DataType: TNotification
    {
        using value_type = TBase;

        using notification_type = TNotification;

        using TNotification::IsEqual;
        using TNotification::Notify;

        DataType() = default;
        explicit DataType(TBase value);
        // explicit DataType(const TBase& value);

        operator const TBase&() const;
        DataType& operator=(TBase value);
        // DataType& operator=(const TBase& value);

    private:
        TBase value;
    };

    template <class T>
    bool Notification::IsEqual(const T& lhs, const T& rhs) const
    {
        return lhs == rhs;
    }

    void Notification::Notify() const
    {
        for (auto& listener : listeners)
        {
            if (listener.func)
            {
                listener.func();
            }
        }
    }

    template <class Tag, class TBase, class TNotification>
    DataType<Tag, TBase, TNotification>::DataType(TBase value)
        : value{value}
    {}

    template <class Tag, class TBase, class TNotification>
    DataType<Tag, TBase, TNotification>::operator const TBase&() const
    {
        return value;
    }

    template <class Tag, class TBase, class TNotification>
    DataType<Tag, TBase, TNotification>& DataType<Tag, TBase, TNotification>::operator=(TBase value)
    {
        if (!IsEqual(this->value, value))
        {
            this->value = value;
            Notify();
        }

        return *this;
    }

#define VALUE_NN(xName, xType) using xName = DataType<struct xName##Tag, xType, NoNotification>;
#define VALUE_NC(xName, xType) using xName = DataType<struct xName##Tag, xType, Notification>;
#include "hal/datamodelcontent.dmc"
#undef VALUE_NN
#undef VALUE_NC

    struct DummyBegin
    {};

    std::tuple<DummyBegin
#define VALUE_NN(xName, xType) , xName
#define VALUE_NC(xName, xType) , xName
#include "hal/datamodelcontent.dmc"
#undef VALUE_NN
#undef VALUE_NC
               >
        datamodel;

    template <class T>
    struct Value
    {
        using value_type = typename T::value_type;

        static void Write(const value_type&);
        static const value_type& Read();

        static void Attach(typename T::notification_type::Callback&);
        static void Detach(typename T::notification_type::Callback&);
    };

    template <class T>
    void Value<T>::Write(const value_type& value)
    {
        std::get<T>(datamodel) = value;
    }

    template <class T>
    auto Value<T>::Read() -> const value_type&
    {
        return std::get<T>(datamodel);
    }

    template <class T>
    void Value<T>::Attach(typename T::notification_type::Callback& cb)
    {
        std::get<T>(datamodel).listeners.push_back(cb);
    }

    template <class T>
    void Value<T>::Detach(typename T::notification_type::Callback& cb)
    {
        std::get<T>(datamodel).listeners.erase(cb);
    }
}

#endif
