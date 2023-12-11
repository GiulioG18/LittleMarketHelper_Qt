
#include "MarketData/DatasetAccess.h"


namespace lmh {

	DatasetAccess::DatasetAccess()
		: dataset_(DatasetRepository::getCurrentDataset())
	{
	}

	DatasetAccess::DatasetAccess(MarketDataset* dataset)
		: dataset_(dataset)
	{
	}

	bool DatasetAccess::availableCurrency(Currency currency) const
	{
		return dataset_->exchangeRateDataset_.availableCurrencies().contains(currency);
	}

}