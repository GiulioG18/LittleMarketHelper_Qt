
// ========================================================================
//		Dataset access class
// ========================================================================

#pragma once

#include "MarketData/MarketDataset.h"
#include "Patterns/Observable.h"


namespace lmh {

	class DatasetAccess : public Observable
	{
	public:

		DatasetAccess(); // Dataset is initialized with the 'current' repository dataset
		DatasetAccess(MarketDataset* dataset);

		// Const methods
		bool availableCurrency(Currency currency) const;

		// Non-const methods


	private:

		MarketDataset* dataset_;
	};

}