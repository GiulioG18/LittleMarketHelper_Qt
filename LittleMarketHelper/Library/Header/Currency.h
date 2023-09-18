// ========================================================================
//		Currencies available
// ========================================================================

#pragma once

#include <string>
#include <map>

#include "Patterns/Observable.h"


namespace lmh {

	enum class Currency
	{
		EUR,
		USD,
		GBP, GBp,
		JPY,
		CHF,
		AUD,
		CAD,
		CNY,
		HKD,
		SEK,
		BZR
	};

	const std::map<Currency, std::string> ccyStringMap =
	{
		{Currency::EUR, "EUR"},
		{Currency::USD, "USD"},
		{Currency::GBP, "GBP"},
		{Currency::GBp, "GBp"},
		{Currency::JPY, "JPY"},
		{Currency::CHF, "CHF"},
		{Currency::AUD, "AUD"},
		{Currency::CAD, "CAD"},
		{Currency::CNY, "CNY"},
		{Currency::HKD, "HKD"},
		{Currency::SEK, "SEK"},
		{Currency::BZR, "BZR"},
	};

}
