
// ========================================================================
//		Curl operations class
// ========================================================================

#pragma once

#include <string>
#include <functional>
#include <memory>

#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "Utils/Assertions.h"
#include "curl/curl.h"

namespace lmh {

	namespace http {

		// Wrapper around libcurl
		// Only HTTP requests are supported
		class Curl : public Singleton<Curl>
		{
		public:

			friend class Singleton<Curl>;

		private:

			Curl() = default;

		public:

			// Following libcurl doc suggestions, in this way multiple calls 
			// to curl_global_init()/curl_global_cleanup() are avoided
			virtual ~Curl();
			// By default sets the flag to CURL_GLOBAL_ALL 
			// NB: no options are set during initialization
			static Status initialize(long flag = -1);
			// Returns string format for CURLcode
			std::string StatusMessage();
			// Check network connectivity
			Status checkNetworkConnection();

			// Requests (Note that returning SUCCESS does not imply that the response is good)
			Status GETRequest(const std::string& url);
			Status POSTRequest(const std::string& url, const std::string& data); // Content-Type: application/json

			// Getters
			inline curl_version_info_data* version() const;
			inline const std::string& response() const;
			inline bool initialized() const;

		private:

			bool initialized_ = false;
			curl_version_info_data* version_ = nullptr;
			std::string response_ = "";
			CURLcode lastCode_ = CURLcode::CURLE_OK;
		};


		// Inline definitions
		inline curl_version_info_data* Curl::version() const { return version_; };
		inline const std::string& Curl::response() const { return response_; };
		inline bool Curl::initialized() const { return initialized_; };

	}

}

