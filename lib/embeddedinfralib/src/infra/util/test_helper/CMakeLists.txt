
if(IS_TESTING)
    target_sources(test_embeddedinfralib PUBLIC
        BoundedStringMatcher.hpp
        OptionalMatcher.hpp
        MemoryRangeMatcher.hpp
        MockCallback.hpp
        MockHelpers.hpp
        MonitoredConstructionObject.hpp
        MoveConstructible.hpp
        ProxyCreatorMock.hpp
        VariantPrintTo.hpp)
endif()