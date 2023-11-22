
#include "Utils/InputValidator.h"
#include "Forex.h"


namespace lmh {

	bool ValidateInput::isin(std::string_view isin)
	{
		// Could be improved, but for now this is enough
		return isin.length() == 12;
	}

	bool ValidateInput::quantity(uint32_t quantity)
	{
		return quantity >= 0;
	}

	bool ValidateInput::currency(Currency currency)
	{
		return Forex::availableCurrency(currency);
	}

	bool ValidateInput::amount(double amount)
	{
		return amount >= 0;
	}

	bool lmh::ValidateInput::recentDate(const Date& date)
	{
		// TODO2: must read from config file what is considered "recent"
		// NB: should count only workdays?
		return false;
	}

}