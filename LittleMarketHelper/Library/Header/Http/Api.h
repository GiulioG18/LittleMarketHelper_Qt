
// ========================================================================
//		Http API utility functions
// ========================================================================

#pragma once

#include <set>
#include <string>
#include <optional>

#include "Utils/StatusCode.h"


namespace lmh {

	// Forward declarations
	enum class Currency;
	class ExchangeRate;
	class Quote;
	class YTicker;


	namespace http {

		class Api
		{

			using Rates = std::set<ExchangeRate>;

		public:

			// Check network connectivity
			static Status testNetworkConnection();
			static Rates fetchLatestRatesFor(Currency currency);
			static std::optional<Quote> fetchLatestQuoteFor(std::string_view isin, Currency currency);
			static std::set<YTicker> fetchYTickersFor(std::string_view isin);

		private:

			static constexpr std::string basePath();
			static void replacePlaceholder(std::string& s, std::string_view value);						
		};

	}

}


/*
		// Get lmh::Quote
		class Quote : public Api
		{
		public:

			Quote();
			virtual ~Quote() = default;


		};


		// Get lmh::ExchangeRate
		class ExchangeRate : public Api
		{
		public:

			ExchangeRate();
			virtual ~ExchangeRate() = default;

			// Returns a list of rates
			std::set<lmh::ExchangeRate> run(Currency baseCcy); 	// [ MAY THROW ]
		};


		// Get a security's ticker (std::string) from an Isin (std::string)
		class Ticker : public Api
		{
		public:

			Ticker();
			virtual ~Ticker() = default;
		};

	}

	
	class YahooFinanceApi : public HttpApi
	{
	public:

		// NB: this function removes tickers from the Json response while trying to get a Quote
		std::optional<Quote> getQuoteFromTicker(const std::string& ticker)
		{
			//// HTTP request
			//std::string figiUrl = Config::read<std::string>("httpRequest.tickerByIsin.openFigi.url");
			//std::string figiData = Config::read<std::string>("httpRequest.tickerByIsin.openFigi.data");
			//ConfigRequest::replacePlaceholder(figiData, isin);
			//*status = Curl::get().POSTRequest(figiUrl, figiData);
			//if (*status != Status::SUCCESS)
			//	return std::nullopt;

			//// Check response format is valid JSON (remove outer square brackets first)
			//Json response;
			//std::stringstream ss;
			//ss << Curl::get().response();
			//*status = response.parse(ss);
			//if (*status != Status::SUCCESS)
			//	return std::nullopt;
			////std::cout << Curl::get().response() << std::endl;

			//// Scan JSON for either data or error node
			//pt::ptree::value_type actual = response.tree().get_
			//	auto it1 = response.tree().get_child().find("error");
			//if (it1 != response.tree().data()
			//	return std::nullopt;

			//	auto it2 = response.tree().find("data");
			//	if (it2 != response.tree().not_found())
			//		return std::nullopt;

			//	// 2. Run yf http request for this ticker.exchange pair

			//	// ...
			return std::nullopt;
		}
		// Calls getQuoteFromTicker internally
		std::optional<Quote> getQuoteFromIsin(const std::string& isin)
		{
			// 1. Run OpenFigi to get a possible ticker for this isin
			OpenFigiApi of;
			auto ticker = of.getTicker(isin);

			return getQuoteFromTicker(ticker.amount());
		}
	};
	*/