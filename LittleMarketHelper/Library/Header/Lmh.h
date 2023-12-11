// ========================================================================
//		Little Market Helper
// ========================================================================

#pragma once

#include "Utils/StatusCode.h"
#include "Config.h"
#include "Utils/Logger.h"
#include "Http/Curl.h"
#include "Currency.h"
#include "MarketData/MarketDataset.h"
#include "Http/Api.h"


namespace lmh {

	// Must be called at startup
	inline Status start(
		const fs::path& configPath,
		LogLevel logLevel = LOG_LEVEL_DEFAULT,
		int64_t curlGlobalFlag = CURL_GLOBAL_ALL
		)
	{
		RETURN_ON_ERROR(Config::initialize(configPath));
		RETURN_ON_ERROR(Logger::initialize(logLevel));
		RETURN_ON_ERROR(http::Curl::initialize(curlGlobalFlag));

		Currency baseCurrency = ccy::stoc(Config::properties().get<std::string>("settings.exchangeRate.baseCurrency")).value();
		RETURN_ON_ERROR(DatasetRepository::initialize(baseCurrency));

		// Check for a valid internet connection
		// Right now the app won't start without it, but later it would be possible
		// to have also an offline available version
		return http::Api::testNetworkConnection();
	}

}