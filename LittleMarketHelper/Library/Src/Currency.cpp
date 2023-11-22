
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
			{Currency::KRW, "KRW"},
			{Currency::SDG, "SDG"},
			{Currency::NOK, "NOK"},
			{Currency::NZD, "NZD"},
			{Currency::INR, "INR"},
			{Currency::MXN, "MXN"},
			{Currency::TWD, "TWD"},
			{Currency::ZAR, "ZAR"},
			{Currency::BRL, "BRL"},
			{Currency::DKK, "DKK"},
			{Currency::PLN, "PLN"},
			{Currency::IDR, "IDR"},
			{Currency::CZK, "CZK"},
			{Currency::HUF, "HUF"},
			{Currency::TRY, "TRY"},
			{Currency::CLP, "CLP"},
			{Currency::RON, "RON"},
			{Currency::ARS, "ARS"},
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