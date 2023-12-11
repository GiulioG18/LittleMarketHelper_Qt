
// ========================================================================
//		Market datasets repository
// ========================================================================

#pragma once

#include <map>

#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "MarketData/ExchangeRatesDataset.h"
#include "MarketData/SecuritiesDataset.h"
#include "MarketData/QuotesDataset.h"
#include "Utils/Chrono.h"


namespace lmh {

	// Forward declarations
	class DatasetRepository;
	class DatasetAccess;



	// Market dataset (dynamic data)

	class MarketDataset
	{
	public:

		friend class DatasetRepository; // TODO: remove
		friend class DatasetAccess;

	public:

		MarketDataset(Currency baseCurrency);
		
		bool isCurrent() const;

	private:
		
		ExchangeRatesDataset exchangeRateDataset_;
		QuotesDataset quotesDataset_;
	};




	// Datasets repository

	class DatasetRepository : public Singleton<DatasetRepository>
	{
	public:

		friend class Singleton<DatasetRepository>;
		 
	private:

		DatasetRepository();

	public:

		static Status initialize(Currency baseCurrency);

		static MarketDataset* const getCurrentDataset();
		static void setCurrentDataset(MarketDataset* mds); // [ MAY THROW ]
		static MarketDataset* const getTodaysDataset();
		static SecuritiesDataset* const getSecurities();

	private:

		void initializeTodaysDataset(Currency baseCurrency);
		 
	private:
		
		SecuritiesDataset securitiesDataset_; // Static data		
		std::map<Date, MarketDataset> mds_;
		MarketDataset* current_;
		bool initialized_;
	};

}
