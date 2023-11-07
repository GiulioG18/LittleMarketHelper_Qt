
#include "Utils/Warnings.h"

#ifdef _WIN32
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "crypt32.Lib")
#pragma comment(lib, "normaliz.lib")
#pragma comment(lib, "ws2_32.Lib")
#pragma comment(lib, "wldap32.Lib")
#else
static_assert(false, "I had no clue nor interest in linking these libs for other OS");
#endif

#include "Http/Curl.h"
#include "Utils/Logger.h"
#include "Http/Api.h"

// Macro to handle Curl errors
#define RETURN_ON_ERROR_CC(x, err) do {	\
  lastCode_ = (x);						\
  if (lastCode_ != CURLE_OK)			\
  {										\
	curl_easy_cleanup(curl);			\
    return err;							\
  }										\
} while (0)



namespace lmh {

	namespace http {

		// Define a callback function for writing data
		size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output)
		{
			size_t totalSize = size * nmemb;
			output->append(static_cast<char*>(contents), totalSize);
			return totalSize;
		}


		// Curl

		Curl::~Curl()
		{
			if (initialized_)
			{
				curl_global_cleanup();
			}
		}

		Status Curl::initialize(long flag)
		{
			Curl& curl = Curl::get();
			if (curl.initialized_)
				return Status::CURL_ALREADY_INITIALIZED;

			// Init curl
			CURLcode code;
			if (flag == -1)
				code = curl_global_init(CURL_GLOBAL_ALL);
			else
				code = curl_global_init(flag);

			if (code != CURLE_OK)
				return Status::CURL_GLOBAL_INIT_FAILED;

			// init version
			curl.version_ = curl_version_info(CURLVERSION_NOW);

			// Finalize
			curl.initialized_ = true;

			// Verify that there is a valid internet connection
			Status status = curl.checkNetworkConnection();
			if (status != Status::SUCCESS)
				curl.initialized_ = false;

			return status;
		}

		std::string Curl::StatusMessage()
		{
			return std::string(curl_easy_strerror(lastCode_));
		}

		Status Curl::checkNetworkConnection()
		{
			if (!initialized_)
				return Status::CURL_NOT_INITIALIZED;

			Status status;
			http::ConnectionTest().run() ?
				status = Status::SUCCESS :
				status = Status::NO_NETWORK_CONNECTION;

			return status;
		}

		// TODO: really should avoid init a new curl easy handle every single time.
		//		 gotta make it work by just setting it up once and reusing it with the common options like writefunction and timeout, 
		//		 while cleaning the rest by setting them to NULL
		Status Curl::GETRequest(const std::string& url)
		{
			if (!initialized_)
				return Status::CURL_NOT_INITIALIZED;

			// Clear the response
			response_.clear();

			CURL* curl;
			curl = curl_easy_init();
			if (!curl)
				return Status::CURL_HANDLE_INIT_FAILED;

			// Set url
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()), Status::CURL_URL_SET_FAILED);

			// Set the callback function to receive the response data
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback), Status::CURL_OPT_SET_FAILED);
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_), Status::CURL_WRITEDATA_SET_FAILED);

			// Set maximum time to complete the operation to 5 seconds
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L), Status::CURL_OPT_SET_FAILED);

			// Perform the HTTP GET request
			RETURN_ON_ERROR_CC(curl_easy_perform(curl), Status::CURL_PERFORM_FAILED);

			// Clean handle and return
			curl_easy_cleanup(curl);
			return Status::SUCCESS;
		}

		Status Curl::POSTRequest(const std::string& url, const std::string& data)
		{
			if (!initialized_)
				return Status::CURL_NOT_INITIALIZED;

			// Clear the response
			response_.clear();

			CURL* curl;
			curl = curl_easy_init();
			if (!curl)
				return Status::CURL_HANDLE_INIT_FAILED;

			// Set url
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()), Status::CURL_URL_SET_FAILED);

			// Make it a POST request and set header and data
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_POST, 1L), Status::CURL_POST_SET_FAILED);
			struct curl_slist* slist = NULL;
			slist = curl_slist_append(slist, "Content-Type: application/json");
			slist = curl_slist_append(slist, "Accept: application/json");
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist), Status::CURL_HEADER_SET_FAILED);
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str()), Status::CURL_DATA_SET_FAILED);

			// Set the callback function to receive the response data
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback), Status::CURL_URL_SET_FAILED);
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_), Status::CURL_WRITEDATA_SET_FAILED);

			// Set maximum time to complete the operation to 5 seconds
			RETURN_ON_ERROR_CC(curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L), Status::CURL_OPT_SET_FAILED);

			// Perform the HTTP GET request
			RETURN_ON_ERROR_CC(curl_easy_perform(curl), Status::CURL_PERFORM_FAILED);

			// Clean handle and return
			curl_easy_cleanup(curl);
			return Status::SUCCESS;
		}

	}

}

