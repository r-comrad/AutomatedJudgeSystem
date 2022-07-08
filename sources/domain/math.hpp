#ifndef DOM_MATH_H
#define DOM_MATH_H

#include "type.hpp"
#include "string.hpp"

namespace dom
{
	class Math
	{
	public:
		static uint_64 getHash(const std::string& aStr);
	};
}

#endif // !DOM_MATH_H