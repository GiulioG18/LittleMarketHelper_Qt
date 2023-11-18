
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

		Quote(const Price& price); // Constructs a Quote with a date with epoch timestamp and current timezone
		Quote(const Price& price, const Date& date);

		// Setters
		inline void setAmount(double amount); // [ MAY THROW ]

		// Getters
		inline const Price& price() const;
		inline const Date& date() const;

	private:

		Price price_;
		Date date_;
	};


	// Inline definitions
	inline void Quote::setAmount(double amount) { price_.set(amount); }
	inline const Price& Quote::price() const { return price_; }
	inline const Date& Quote::date() const { return date_; }

}