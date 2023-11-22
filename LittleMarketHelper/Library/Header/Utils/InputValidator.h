
// ========================================================================
//		Set of helper function to validate user input
// ========================================================================

#pragma once

#include <string>


namespace lmh {

	// Forward declarations
	class Date;
	enum class Currency;


	class ValidateInput
	{
	public:

		static bool isin(std::string_view isin);
		static bool quantity(uint32_t quantity);
		static bool currency(Currency currency);
		static bool amount(double amount);
		static bool recentDate(const Date& date);
	};

}