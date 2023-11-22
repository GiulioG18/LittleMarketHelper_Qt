
#include <boost/algorithm/string.hpp>

#include "Http/Api.h"
#include "Http/Curl.h"
#include "Config.h"
#include "ExchangeRate.h"
#include "Currency.h"
#include "Quote.h"
#include "Ticker.h"
#include "Utils/Cache.h"
#include "Utils/Json.h"


namespace lmh {

	namespace http {

		Status Api::testNetworkConnection()
		{
			assert(Curl::get().initialized());

			// Initialize request
			std::string path = basePath() + "networkConnection.";
			Curl::Method method = Curl::stom(Config::properties().get<std::string>(path + "method"));
			std::string url = Config::properties().get<std::string>(path + "url");
			std::string key = Config::properties().get<std::string>(path + "key");

			// Force the HTTP request every time
			static Curl::Request request{ method, url, "", true };
			auto response = Curl::httpRequest(request);

			// Process response
			auto json = response.toJson();
			if (!json.has_value())
				return Status::NO_NETWORK_CONNECTION;

			// Evaluate message
			auto msg = json.value().tree().get_optional<std::string>(key);
			if (msg.has_value()) // NB: a very cool message is lost here
				return Status::SUCCESS;
			else
				return Status::NO_NETWORK_CONNECTION;
		}

		Api::Rates Api::fetchRatesFor(Currency currency)
		{
			assert(Curl::get().initialized());

			std::set<lmh::ExchangeRate> out;

			// Initialize request
			std::string path = basePath() + "exchangeRates.";
			Curl::Method method = Curl::stom(Config::properties().get<std::string>(path + "method"));
			std::string url = Config::properties().get<std::string>(path + "url");
			std::string key = Config::properties().get<std::string>(path + "key");

			// Run HTTP request
			static Curl::Request request{ method , url, "" };
			auto response = Curl::httpRequest(request);

			// Process response
			auto json = response.toJson();
			if (!json.has_value())
				return out;

			// Request rates for most common currencies
			Currency xxx = currency;
			for (const auto& pair : ccy::map())
			{			
				Currency yyy = pair.first;
				std::string currency = pair.second;
				boost::algorithm::to_lower(currency);
				auto rate = json.value().tree().get_optional<double>(key + "." + currency);

				// Push new exchange rate into set only if rate is found in the response data
				if (rate.has_value())
					out.emplace(xxx, yyy, rate.value());
			}

			// TODO2: should check if the date is recent enough
			return out;
		}

		std::optional<Quote> Api::fetchQuoteFor(std::string_view isin, Currency currency)
		{
			// TODO: impl
			assert(Curl::get().initialized());

			auto tickers = fetchYTickersFor(isin);
			for (const auto& ticker : tickers)
			{
				//		_ Request chart yf
				//		_ store results
				//		_ if 3 prices meet -> return average.

			}

			return std::nullopt;
		}

		std::set<YTicker> Api::fetchYTickersFor(std::string_view isin)
		{
			assert(Curl::get().initialized());

			// TODO: impl

			// Check in the cache first
			static std::unique_ptr<Cache<std::string_view, std::set<YTicker>>> cache = std::make_unique<FifoCache<std::string_view, std::set<YTicker>>>();
			auto cachedJson = cache->get(isin);
			if (cachedJson)
			{

			}

			// 1. Run openFIGI request
			// 2. Cache the json response (validated)
			// 2. 

			return {};
		}

		constexpr std::string Api::basePath()
		{
			return std::string("httpRequest.");
		}

		void Api::replacePlaceholder(std::string& s, std::string_view value)
		{
			std::string placeholder = std::string("%%%PLACEHOLDER%%%");

			auto pos = s.find(placeholder);
			if (pos == std::string::npos)
				return;

			s.replace(pos, placeholder.length(), value);
		}
	}

}
