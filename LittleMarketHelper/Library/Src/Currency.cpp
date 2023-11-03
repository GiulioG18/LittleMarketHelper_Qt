
#include "Currency.h"


namespace lmh {

	const std::map<Currency::Type, std::string> _currencyStringMap =
	{
		{Currency::Type::EUR, "EUR"},
		{Currency::Type::USD, "USD"},
		{Currency::Type::GBP, "GBP"},
		{Currency::Type::JPY, "JPY"},
		{Currency::Type::CHF, "CHF"},
		{Currency::Type::AUD, "AUD"},
		{Currency::Type::CAD, "CAD"},
		{Currency::Type::CNY, "CNY"},
		{Currency::Type::HKD, "HKD"},
		{Currency::Type::SEK, "SEK"},
		{Currency::Type::BZR, "BZR"},
	};

	std::optional<Currency::Type> Currency::fromString(const std::string& ccyStr)
	{
		std::optional<Type> ccy = std::nullopt;

		for (const auto& keyValue : _currencyStringMap)
		{
			if (keyValue.second == ccyStr)
			{
				ccy = keyValue.first;
				break;
			}
		}

		return ccy;
	}

	std::string Currency::toString(Type ccy)
	{
		return _currencyStringMap.at(ccy);
	}

	const std::map<Currency::Type, std::string>& lmh::Currency::map()
	{
		return _currencyStringMap;
	}

}