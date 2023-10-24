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

		using Ccy = Currency::Type;

		struct Comp;

	public:

		// Constructor [ MAY THROW ]
		Cash(Ccy ccy, double value);

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
		bool operator()(const Ccy&	first,	const Cash& second) const;
		bool operator()(const Cash& first,  const Ccy&	second) const;
	};


	// Inline definitions
	inline const Price& Cash::price() const { return price_; }
	inline void Cash::setValue(double amount) { price_.set(amount); }
}