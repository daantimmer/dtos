#pragma once

namespace elib
{
    struct function
    {
    private:
        static constexpr auto fakelambda = [v = nullptr]() {};

        struct IFunctor
        {
            virtual void operator()() = 0;
        };

        template<typename T>
        struct FunctorLambdaImpl : IFunctor
        {
            FunctorLambdaImpl(T lambdaImpl)
                : lambda(lambdaImpl)
            {
            }

            virtual void operator()() override
            {
                lambda();
            }

        private:
            T lambda;
        };

        template<typename T, void (T::*func)()>
        struct FunctorMemberImpl : IFunctor
        {
            FunctorMemberImpl(T* objectPtr)
                : object(objectPtr)
            {
            }

            virtual void operator()() override
            {
                (object->*func)();
            }

        private:
            T* object;
        };

        struct FunctorFreeImpl : IFunctor
        {
            using fpointer = void (*)();

            FunctorFreeImpl(fpointer fp)
                : freeFunctionPointer(fp)
            {
            }

            virtual void operator()() override
            {
                freeFunctionPointer();
            }

        private:
            fpointer freeFunctionPointer;
        };

        union MemoryObj
        {
            using MemberImpl = FunctorMemberImpl<IFunctor, &IFunctor::operator()>;
            using LambdaImpl = FunctorLambdaImpl<decltype(fakelambda)>;
            using FreeImpl = FunctorFreeImpl;

            alignas(alignof(MemberImpl)) std::array<uint8_t, sizeof(MemberImpl)> memoryMemberImpl;
            alignas(alignof(MemberImpl)) std::array<uint8_t, sizeof(LambdaImpl)> memoryLambdaImpl;
            alignas(alignof(MemberImpl)) std::array<uint8_t, sizeof(FreeImpl)> memoryFreeImpl;
        };

    public:
        function()
        {
        }

		explicit function(FunctorFreeImpl::fpointer fpointer)
        {
            new (memory.memoryLambdaImpl.data()) FunctorFreeImpl(fpointer);
		}

        template<typename T>
        explicit function(T lambda)
		{
            static_assert(sizeof(FunctorLambdaImpl<T>) <= sizeof(MemoryObj::LambdaImpl), "Lambda doesn't fit");

			new (memory.memoryLambdaImpl.data()) FunctorLambdaImpl<T>(lambda);
		}

		template<typename T, void (T::*func)()>
		static function Create(T* object)
		{
            function retfunc;

			new (retfunc.memory.memoryMemberImpl.data()) FunctorMemberImpl<T, func>(object);

			return retfunc;
		}

		void operator()()
		{
            reinterpret_cast<IFunctor*>(memory.memoryMemberImpl.data())->operator()();
		}


	private:
        MemoryObj memory;
    };
} // namespace elib