
// ========================================================================
//		Status returned
// ========================================================================

#pragma once

#include <string>

// Macro to handle Curl errors
#define RETURN_ON_ERROR(x) do {	\
  Status s = (x);				\
  if (s != Status::SUCCESS)		\
    return s;					\
} while (0)


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
		// Mdsr has already been initialized in this session
		MDSR_ALREADY_INITIALIZED,
	};


	namespace error {

		// TODO: impl
		inline std::string stos(Status status)
		{
			switch (status)
			{
			case lmh::Status::SUCCESS:
				break;
			case lmh::Status::CURRENCY_NOT_ALLOWED:
				break;
			case lmh::Status::TRADE_NOT_FOUND:
				break;
			case lmh::Status::CASH_NOT_FOUND:
				break;
			case lmh::Status::TRADE_DUPLICATE_NOT_INSERTED:
				break;
			case lmh::Status::CASH_DUPLICATE_NOT_INSERTED:
				break;
			case lmh::Status::INVALID_INPUT:
				break;
			case lmh::Status::FILE_NOT_OPEN:
				break;
			case lmh::Status::FILE_IS_EMPTY:
				break;
			case lmh::Status::REPORT_NOT_FOUND:
				break;
			case lmh::Status::INVALID_DIRECTORY:
				break;
			case lmh::Status::NO_WRITE_PERMISSION:
				break;
			case lmh::Status::INVALID_JSON_FORMAT:
				break;
			case lmh::Status::CURL_NOT_INITIALIZED:
				break;
			case lmh::Status::CURL_ALREADY_INITIALIZED:
				break;
			case lmh::Status::CURL_GLOBAL_INIT_FAILED:
				break;
			case lmh::Status::CURL_HANDLE_INIT_FAILED:
				break;
			case lmh::Status::CURL_REQUEST_FAILED:
				break;
			case lmh::Status::CURL_URL_SET_FAILED:
				break;
			case lmh::Status::CURL_POST_SET_FAILED:
				break;
			case lmh::Status::CURL_DATA_SET_FAILED:
				break;
			case lmh::Status::CURL_HEADER_SET_FAILED:
				break;
			case lmh::Status::CURL_WRITEFUNCTION_SET_FAILED:
				break;
			case lmh::Status::CURL_WRITEDATA_SET_FAILED:
				break;
			case lmh::Status::CURL_OPT_SET_FAILED:
				break;
			case lmh::Status::CURL_PERFORM_FAILED:
				break;
			case lmh::Status::CONFIG_NOT_INITIALIZED:
				break;
			case lmh::Status::CONFIG_ALREADY_INITIALIZED:
				break;
			case lmh::Status::CONFIG_INVALID_PATH:
				break;
			case lmh::Status::API_INVALID_METHOD:
				break;
			case lmh::Status::LOGGER_ALREADY_INITIALIZED:
				break;
			case lmh::Status::NO_NETWORK_CONNECTION:
				break;
			default:
				break;
			}

			return std::string("Not implemented yet!!");
		}

	}

}

