#include "kernel/container/heap.hpp"
#include <gmock/gmock.h>

TEST(TestHeap, tstt)
{
    kernel::Heap<int>::WithSize<3> heap;

    auto a = heap.Claim();
    auto b = heap.TryClaim();
    auto c = heap.TryClaim();
    auto d = heap.TryClaim();

    heap.Release(a.Get<std::reference_wrapper<int>>().get());
}