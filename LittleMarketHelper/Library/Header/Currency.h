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
			GBp,
			JPY,
			CHF,
			AUD,
			CAD,
			CNY,
			HKD,
			SEK,
			BZR
		};

		static inline std::optional<Type> get(const std::string& ccyStr);

	public:

		static const std::map<Type, std::string> _typeMap;
	};

	// Inline definitions

	inline std::optional<Currency::Type> Currency::get(const std::string& ccyStr)
	{
		std::optional<Type> ccy = std::nullopt;

		for (const auto& keyValue : _typeMap)
		{
			if (keyValue.second == ccyStr)
			{
				ccy = keyValue.first;
				break;
			}
		}

		return ccy;
	}

}
