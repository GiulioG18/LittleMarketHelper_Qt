// ========================================================================
//		Little Market Helper
// ========================================================================

#pragma once

#include "Utils/StatusCode.h"
#include "Config.h"
#include "Utils/Logger.h"
#include "Http/Curl.h"
#include "ExchangeRate.h"


namespace lmh {

	inline Status start(
		const fs::path& configPath,
		Currency baseCurrency = Currency::EUR,
		LogLevel logLevel = LOG_LEVEL_DEFAULT,
		long curlGlobalFlag = CURL_GLOBAL_ALL
		)
	{
		RETURN_ON_ERROR(Config::initialize(configPath));
		RETURN_ON_ERROR(Logger::initialize(logLevel));
		RETURN_ON_ERROR(http::Curl::initialize(curlGlobalFlag));
		RETURN_ON_ERROR(ExchangeRateRepository::initialize(baseCurrency));

		return Status::SUCCESS;
	}

}