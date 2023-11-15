
// ========================================================================
//		Security quote
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Price.h"
#include "Utils/Chrono.h"


namespace lmh {

	class Quote 
	{
	public:

		Quote(const Price& price); // [ MAY THROW ]
		Quote(const Price& price, const Date& date); // [ MAY THROW ]

		// Setters
		inline void setValue(double amount); // [ MAY THROW ]

		// Getters
		inline const Price& price() const;
		inline const Date& date() const;

	private:

		Price price_;
		Date date_;
	};


	// Inline definitions
	inline void Quote::setValue(double amount) { price_.set(amount); }
	inline const Price& Quote::price() const { return price_; }
	inline const Date& Quote::date() const { return date_; }

}