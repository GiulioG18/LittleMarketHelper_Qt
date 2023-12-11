
// ========================================================================
//		Type traits helper
// ========================================================================

#pragma once

#include <type_traits>


namespace lmh {

	// Forward declarations
	class Observable;
	class MarketValue;


	// To ignore const and ref when comparing types
	template<class T1, class T2>
	bool is_almost_same_v = std::is_same_v<const T1&, const T2&>;

	// Concept: Type must derive from Observable
	template <typename Type>
	concept IsObservable = std::is_base_of<Observable, Type>::value;

	// Concept: Type must have a method signed "MarketValue accumulate()"
	template<typename Type>
	concept HasAccumulate = requires(Type instance)
	{
		{ instance.accumulate() } -> std::convertible_to<MarketValue>;
	};

}