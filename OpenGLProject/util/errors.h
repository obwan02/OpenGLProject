#pragma once
#include <oglpch.h>

namespace ogl {
    class ErrNotImplemented : public std::logic_error {
    public:
        ErrNotImplemented() : std::logic_error("Function not yet implemented") { };
    };
}

