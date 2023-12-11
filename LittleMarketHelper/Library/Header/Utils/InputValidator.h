
// ========================================================================
//		Set of helper function to validate user input
// ========================================================================

#pragma once

#include <string>


namespace lmh {

	// Forward declarations
	enum class Currency;
	class MarketValue;
	class Date;
	class Quote;


	class ValidateInput
	{
	public:

		static bool isin(std::string_view isin);
		static bool quantity(uint32_t quantity);
		static bool currency(Currency currency);
		static bool marketValue(const MarketValue& mv);
		static bool recentDate(const Date& date);
		static bool quote(const Quote& quote);
	};

}