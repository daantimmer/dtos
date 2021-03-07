#include "infra/util/Function.hpp"

namespace infra
{
    const infra::Function<void()> emptyFunction = []() {};

    Execute::Execute(const Function<void()>& f)
    {
        f();
    }

    ExecuteOnDestruction::ExecuteOnDestruction(const Function<void()>& f)
        : f(f)
    {}

    ExecuteOnDestruction::~ExecuteOnDestruction()
    {
        f();
    }
}
