
#include "ExchangeRate.h"
#include "Http/Api.h"


namespace lmh {

	// ExchangeRate

	ExchangeRate::ExchangeRate(std::string denomination, double value)
		: denomination_(denomination), value_(value)
	{
		// TODO: impl
		FAIL("not implemented");
		REQUIRE(xxx_ != yyy_, "same currency provided for exchange rate currencies");
	}

	ExchangeRate::ExchangeRate(Currency xxx, Currency yyy, double value)
		:
		xxx_(xxx),
		yyy_(yyy),
		value_(value),
		denomination_(Forex::denomination(xxx, yyy))
	{
		REQUIRE(value_ > 0, "exchange rate provided is not positive");
		if (xxx_ == yyy_) 
			REQUIRE(value_ == 1.0, "same currency exchange rate is not 1");
	}

	bool ExchangeRate::operator<(const ExchangeRate& other) const
	{
		return this->denomination_ < other.denomination_;
	}


	// ExchangeRateRepository

	Status ExchangeRateRepository::initialize(Currency baseCurrency)
	{
		ExchangeRateRepository& repo = get();
		if (repo.initialized_)
			return Status::ER_REPO_ALREADY_INITIALIZED;

		// Initialize base currency
		repo.baseCurrency_ = baseCurrency;

		// Request rates and fill the maps
		std::set<ExchangeRate> rates = http::ExchangeRate().run();
		for (auto& rate : rates)
		{
			// Conversion between non-base currencies are triangulated using the
			//  base one, therefore only base currency rates are stored
			if (rate.xxx() != repo.baseCurrency_)
				continue;

			// Skip non-positive rate
			if (rate.value() <= 0.0)
				continue;

			// Insert rate into maps
			bool inserted = repo.rates_.insert({ Forex::denomination(rate.xxx(), rate.yyy()), std::move(rate) }).second;

			// Mark currency as available
			if (inserted)
				repo.availableCurrencies_.insert(rate.yyy());
		}

		ENSURE(repo.availableCurrencies_.find(repo.baseCurrency_) != repo.availableCurrencies_.end(), "base currency not registered as available");

		repo.initialized_ = true;
		return Status::SUCCESS;
	}
}