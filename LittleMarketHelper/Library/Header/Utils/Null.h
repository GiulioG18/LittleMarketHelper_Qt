
// ========================================================================
//		Null value
// ========================================================================

#pragma once

#include <limits>
#include <type_traits>


namespace lmh {

	// Forward declarations
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

		static_assert(std::is_arithmetic<Type>::value, "Only arithmetic types are allowed");
		static_assert(!std::is_same<Type, bool>::value, "Booleans not allowed");

	public:

		Null() = default;

		constexpr operator Type() const
		{
			return Type(FloatingPoint<std::is_floating_point<Type>::value>::maxValue());
		}

		static inline bool check(Type value)
		{
			return value == Null<Type>();
		}
	};

}
