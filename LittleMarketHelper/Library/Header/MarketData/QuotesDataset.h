
// ========================================================================
//		Quotes dataset
// ========================================================================

#pragma once

#include <map>

#include "Currency.h"


namespace lmh {

	// Forward declarations
	class Quote;
	class Security;


	class QuotesDataset
	{
	public:

		QuotesDataset(Currency baseCurrency);

	private:
		
		std::map<Security*, const Quote> quotes_;
		Currency baseCurrency_;
	};

}