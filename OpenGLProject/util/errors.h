#pragma once
#include <stdexcept>

namespace ogl {
    class ErrNotImplemented : public std::logic_error {
    public:
        ErrNotImplemented() : std::logic_error("Function not yet implemented") { };
    };
}

