// ========================================================================
//		Currencies available
// ========================================================================

#pragma once

#include <string>
#include <map>
#include <optional>

#include "Patterns/Observable.h"


namespace lmh {

	class Currency
	{
	public:

		enum class Type
		{
			EUR,
			USD,
			GBP,
			JPY,
			CHF,
			AUD,
			CAD,
			CNY,
			HKD,
			SEK,
			BZR
		};

		static std::optional<Type> fromString(const std::string& ccyStr);
		static std::string toString(Type ccy);
		static const std::map<Currency::Type, std::string>& map();
	};

}
