
// ========================================================================
//		Status returned
// ========================================================================

#pragma once

#include <string>


namespace lmh {

	enum class Status
	{
		// All is good
		SUCCESS = 0,
		// The selected Currency is not allowed
		CURRENCY_NOT_ALLOWED,
		// The trade was not found
		TRADE_NOT_FOUND,
		// Cash was not found
		CASH_NOT_FOUND,
		// The trade was a duplicate
		TRADE_DUPLICATE_NOT_INSERTED,
		// Cash with same currency already exists
		CASH_DUPLICATE_NOT_INSERTED,
		// Invalid input for current function
		INVALID_INPUT,
		// File did not open correctly
		FILE_NOT_OPEN,
		// File is empty
		FILE_IS_EMPTY,
		// Report file is not found
		REPORT_NOT_FOUND,
		// The directory specified is not valid
		INVALID_DIRECTORY,
		// No write permission for the path specified
		NO_WRITE_PERMISSION,
		// File is not a valid JSON
		INVALID_JSON_FORMAT,
		// Curl has not been initialized
		CURL_NOT_INITIALIZED,
		// Curl has already been initialized in this session
		CURL_ALREADY_INITIALIZED,
		// Global Curl failed to initialize
		CURL_GLOBAL_INIT_FAILED,
		// Curl handle failed to initialize
		CURL_HANDLE_INIT_FAILED,
		// Curl request was not successful
		CURL_REQUEST_FAILED,
		// Curl URL setopt was not successful
		CURL_URL_SET_FAILED,
		// Curl Post request setopt was not successful
		CURL_POST_SET_FAILED,
		// Curl Data setopt was not successful
		CURL_DATA_SET_FAILED,
		// Curl Header setopt was not successful
		CURL_HEADER_SET_FAILED,
		// Curl Write function setopt was not successful
		CURL_WRITEFUNCTION_SET_FAILED,
		// Curl Write data setopt was not successful
		CURL_WRITEDATA_SET_FAILED,
		// Curl setopt failed
		CURL_OPT_SET_FAILED,
		// Curl perform was not succesfull
		CURL_PERFORM_FAILED,
		// Config has not been initialized
		CONFIG_NOT_INITIALIZED,
		// Config has already already been initialized in this session
		CONFIG_ALREADY_INITIALIZED,
		// Path to value is not valid
		CONFIG_INVALID_PATH,
		// Invalid method
		API_INVALID_METHOD,
		// Logger has already been initialized in this session
		LOGGER_ALREADY_INITIALIZED,
		// No network connection
		NO_NETWORK_CONNECTION,
	};

	inline std::string StatusToString(Status Status)
	{
		switch (Status)
		{
		case Status::SUCCESS:
			break;
		case Status::CURRENCY_NOT_ALLOWED:
			break;
		case Status::TRADE_NOT_FOUND:
			break;
		case Status::INVALID_INPUT:
			break;
		case Status::FILE_NOT_OPEN:
			break;
		case Status::FILE_IS_EMPTY:
			break;
		case Status::REPORT_NOT_FOUND:
			break;
		default: return std::string("Unknown error"); break;
		}
	}

}

