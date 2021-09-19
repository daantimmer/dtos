// Copyright (c) 2015 Koninklijke Philips N.V.
#include <cassert>
#include <cstdlib>

void* operator new(std::size_t n)
{
    assert(false);
    return 0;
}

void* operator new[](std::size_t n)
{
    assert(false);
    return 0;
}

void* operator new(std::size_t size_, void* p) throw()
{
    return p;
}

void* operator new[](std::size_t n, void* p) throw()
{
    return p;
}

void operator delete(void* p) throw()
{}

void operator delete[](void* p) throw()
{}

void operator delete(void*, void*) throw()
{}

void operator delete[](void*, void*) throw()
{}

void operator delete(void*, unsigned int) throw()
{}

void operator delete[](void*, unsigned int) throw()
{}