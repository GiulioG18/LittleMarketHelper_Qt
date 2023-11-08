
#include "Currency.h"


namespace lmh {

	namespace ccy {

		const std::map<Currency, std::string> _currencies =
		{
			{Currency::EUR, "EUR"},
			{Currency::USD, "USD"},
			{Currency::GBP, "GBP"},
			{Currency::JPY, "JPY"},
			{Currency::CHF, "CHF"},
			{Currency::AUD, "AUD"},
			{Currency::CAD, "CAD"},
			{Currency::CNY, "CNY"},
			{Currency::HKD, "HKD"},
			{Currency::SEK, "SEK"},
			{Currency::BZR, "BZR"},
		};

		std::optional<Currency> stoc(std::string_view currencyStr)
		{
			std::optional<Currency> currency = std::nullopt;

			for (const auto& keyValue : _currencies)
			{
				if (keyValue.second == currencyStr)
				{
					currency = keyValue.first;
					break;
				}
			}

			return currency;
		}

		std::string ctos(Currency currency)
		{
			return _currencies.at(currency);
		}

		const std::map<Currency, std::string>& map()
		{
			return _currencies;
		}

	}	

}