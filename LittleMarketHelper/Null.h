
// ========================================================================
//		Null value
// ========================================================================

#pragma once

#include <limits>
#include <type_traits>


namespace lmh {

	template<bool>
	struct FloatingPoint;

	template<>
	struct FloatingPoint<true>
	{
		constexpr static float maxValue()
		{
			return (std::numeric_limits<float>::max)();
		}
	};

	template<>
	struct FloatingPoint<false>
	{
		constexpr static int maxValue()
		{
			return (std::numeric_limits<int>::max)();
		}
	};

	template<typename Type>
	class Null
	{
	public:

		Null() = default;

		operator Type() const
		{
			return Type(FloatingPoint<std::is_floating_point<Type>::value>::maxValue());
		}
	};

}
