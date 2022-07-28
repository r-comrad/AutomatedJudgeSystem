#ifndef DOM_MATH_HPP
#define DOM_MATH_HPP

//--------------------------------------------------------------------------------

#include <string>

#include "type.hpp"

//--------------------------------------------------------------------------------

namespace dom
{
    class Math
    {
    public:
        static uint_64 getHash(const std::string& aStr) const noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !DOM_MATH_HPP
