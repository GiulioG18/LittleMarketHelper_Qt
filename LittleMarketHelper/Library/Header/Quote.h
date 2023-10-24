
// ========================================================================
//		Security quote
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Price.h"


namespace lmh {

	class Quote 
	{
	public:

		// Constructor [ MAY THROW ]
		Quote(const Price& price, double timestamp);

		// Setters [ MAY THROW ]
		inline void setValue(double amount);

		// Getters
		inline const Price& price() const;
		inline double timestamp() const;

	private:

		Price price_;
		double timestamp_; // Const // TODO: Find appropriate type
	};


	// Inline definitions
	inline void Quote::setValue(double amount) { price_.set(amount); }
	inline const Price& Quote::price() const { return price_; }
	inline double Quote::timestamp() const { return timestamp_; }

}