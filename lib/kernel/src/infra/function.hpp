#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <new>
#include <type_traits>

namespace infra
{
    template<class T>
    struct Function;

    template<class TRet, class... Args>
    struct Function<TRet(Args...)>
    {
        using FuncPtr = TRet (*)(Args...);

        Function()
        {
            new (&memory) EmptyFunction;
        }

        Function(FuncPtr funcPtr)
        {
            new (&memory) FreeFunction {funcPtr};
        }

        template<class Lambda>
        Function(Lambda lambda)
        {
            new (&memory) LambdaFunction {lambda};
        }

        TRet operator()(Args&&... args)
        {
            return reinterpret_cast<IFunction*>(&memory)->operator()(std::forward<Args>(args)...);
        }

    private:
        struct IFunction
        {
            virtual TRet operator()(Args&&...) = 0;
        };

        template<class Lambda>
        struct LambdaFunction : IFunction
        {
            Lambda lambda;

            LambdaFunction(Lambda lambda)
                : lambda(lambda)
            {
                static_assert(sizeof(decltype(*this)) <= sizeof(EmptyFunction));
            }

            virtual TRet operator()(Args&&... args)
            {
                return lambda(std::forward<Args>(args)...);
            }
        };

        struct FreeFunction : IFunction
        {
            FuncPtr ptr;

            FreeFunction(FuncPtr ptr)
                : ptr(ptr)
            {
                static_assert(sizeof(decltype(*this)) <= sizeof(EmptyFunction));
            }

            virtual TRet operator()(Args&&... args)
            {
                return ptr(std::forward<Args>(args)...);
            }
        };

        struct EmptyFunction : IFunction
        {
            void* unused = nullptr;

            virtual TRet operator()(Args&&...)
            {
                if constexpr (!std::is_same_v<TRet, void>)
                {
                    return {};
                }
            }
        };

        std::aligned_storage_t<sizeof(EmptyFunction), alignof(EmptyFunction)> memory;
    };
}

#endif