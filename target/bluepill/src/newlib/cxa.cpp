// Copyright (c) 2015 Koninklijke Philips N.V.
extern "C"
{
    void _init()
    {}

    void _fini()
    {}

    void* __dso_handle = 0;

    __extension__ typedef int __guard __attribute__((mode(__DI__)));

    int __cxa_guard_acquire(__guard* g)
    {
        return !*(char*)(g);
    }
    void __cxa_guard_release(__guard* g)
    {
        *(char*)g = 1;
    }
    void __cxa_guard_abort(__guard*)
    {}
    void __cxa_pure_virtual(void)
    {
        while (true)
        {}
    }
    // This function is a dummy to replace the real exit function
    // The dummy function is created to eliminate the SRAM impure data
    // This function is normally called when C++ code exits termination
    // Our main loop runs in a while(1) loop, so it never exits
    int atexit(void (*func)(void))
    {
        return 0;
    }
}
