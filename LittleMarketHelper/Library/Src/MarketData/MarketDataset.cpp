
#include <cassert>

#include "Utils/Assertions.h"
#include "MarketData/MarketDataset.h"
#include "Config.h"
#include "Http/Curl.h"


namespace lmh {

	// Market dataset

	MarketDataset::MarketDataset(Currency baseCurrency)
		: exchangeRateDataset_(baseCurrency), quotesDataset_(baseCurrency)
	{
	}

	bool MarketDataset::isCurrent() const
	{
		return this == DatasetRepository::getCurrentDataset();
	}




	// Dataset repository

	DatasetRepository::DatasetRepository()
		: current_(nullptr), initialized_(false)
	{
	}

	Status DatasetRepository::initialize(Currency baseCurrency)
	{
		assert(Config::get().initialized());
		assert(http::Curl::get().initialized());

		auto& mdsRepo = DatasetRepository::get();

		// Avoid multiple initializations
		if (mdsRepo.initialized_)
			return Status::MDSR_ALREADY_INITIALIZED;
		
		// Initialize today's market dataset
		mdsRepo.initializeTodaysDataset(baseCurrency);

		// Initialize current dataset pointer to today's dataset
		mdsRepo.setCurrentDataset(mdsRepo.getTodaysDataset());
		
		// Finalize
		mdsRepo.initialized_ = true;

		return Status::SUCCESS;
	}

	MarketDataset* const DatasetRepository::getCurrentDataset()
	{
		assert(DatasetRepository::get().current_);
		return DatasetRepository::get().current_;
	}

	void DatasetRepository::setCurrentDataset(MarketDataset* mds)
	{
		REQUIRE(DatasetRepository::get().current_, "invalid input dataset");
		DatasetRepository::get().current_ = mds;
	}

	MarketDataset* const DatasetRepository::getTodaysDataset()
	{
		// Get today's date
		Date today = Chrono::today();
		
		// Look for today's dataset
		auto it = DatasetRepository::get().mds_.find(today);
		assert(it != std::end(DatasetRepository::get().mds_)); // TODO2: if the session goes past midnight, there will be an issue here
		
		return &it->second;
	}

	SecuritiesDataset* const DatasetRepository::getSecurities()
	{
		return &DatasetRepository::get().securitiesDataset_;
	}

	void DatasetRepository::initializeTodaysDataset(Currency baseCurrency)
	{
		// Get today's date
		Date today = Chrono::today();

		// Create today's dataset
		auto emplacement = mds_.emplace(Chrono::today(), baseCurrency);
		assert(emplacement.second);

		// Initialize today's exchange rates dataset
		emplacement.first->second.exchangeRateDataset_.populateWithLatestQuotes();
	}

}