
#include "Currency.h"


namespace lmh {

	const std::map<Currency::Type, std::string> Currency::_typeMap =
	{
		{Type::EUR, "EUR"},
		{Type::USD, "USD"},
		{Type::GBP, "GBP"},
		{Type::GBp, "GBp"},
		{Type::JPY, "JPY"},
		{Type::CHF, "CHF"},
		{Type::AUD, "AUD"},
		{Type::CAD, "CAD"},
		{Type::CNY, "CNY"},
		{Type::HKD, "HKD"},
		{Type::SEK, "SEK"},
		{Type::BZR, "BZR"},
	};

}