#include <gmock/gmock.h>
#include <infra/function.hpp>

TEST(TestFunction, LambdaNoCapture)
{
    auto myLambda = [](int i) -> int {
        EXPECT_EQ(10, i);
        return i - 1;
    };

    infra::Function<int(int)> func1(myLambda);

    EXPECT_EQ(9, func1(10));
}

struct S
{
    S(int value)
        : value(value)
    {
    }

    auto Get()
    {
        return [this](int add) { return value + add; };
    }

private:
    int value;
};
TEST(TestFunction, LambdaCaptureThis)
{
    S s(10);

	infra::Function<int(int)> func(s.Get());

	EXPECT_EQ(15, func(5));
}

void FreeFunc(int i)
{
    EXPECT_EQ(50, i);
}

TEST(TestFunction, freeFunction)
{
    infra::Function<void(int)> func(&FreeFunc);

    func(50);
}

TEST(TestFunction, DoesNotThrowOnDefaultConstruction)
{
    infra::Function<int(int)> func1;
    infra::Function<void()> func2;

    EXPECT_EQ(0, func1(25));

    func2();
}