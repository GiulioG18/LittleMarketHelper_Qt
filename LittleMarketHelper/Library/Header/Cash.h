// ========================================================================
//		Cash
// ========================================================================

#pragma once

#include "Currency.h"
#include "Price.h"


namespace lmh {

	class Cash
	{
	public:

		struct Comp;

	public:

		Cash(Currency Currency, double value); // [ MAY THROW ]

		// Getters
		inline const Price& price() const;

		// Setters
		inline void setValue(double amount);

	private:

		Price price_;
	};

	// This comparison functor serves to not allow multiple cash objects with same 
	// currency but different values inside a set of Cash pointers
	struct Cash::Comp
	{
		// Transparent comparator
		using is_transparent = void;

		bool operator()(const Cash& first,	const Cash& second) const;
		bool operator()(const Currency&	first,	const Cash& second) const;
		bool operator()(const Cash& first,  const Currency&	second) const;
	};


	// Inline definitions
	inline const Price& Cash::price() const { return price_; }
	inline void Cash::setValue(double amount) { price_.set(amount); }
}