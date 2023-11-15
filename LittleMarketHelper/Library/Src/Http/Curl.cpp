
#include <boost/algorithm/string.hpp>

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
#include "Config.h"
#include "Http/Api.h"


// Helper macro to handle Curl errors
#define CURL_CALL(curl_function, lmh_error) 								\
	(curl_function) == CURLcode::CURLE_OK ? Status::SUCCESS : lmh_error		\

// Helper macro to return on invalid response
#define RETURN_ON_INVALID_RESPONSE						\
	if (response.code_ != Status::SUCCESS)				\
	{													\
		curl_easy_cleanup(curl);						\
		return;											\
	}													\



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

		Status Curl::initialize(int64_t flag)
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

			// Init Cache
			curl.responses_ = std::make_unique<FifoCache<Request, Response>>(30); // 30 max request are cached at the same time

			// Finalize
			curl.initialized_ = true;
			return Status::SUCCESS;
		}

		Curl::Response Curl::httpRequest(const Curl::Request& request)
		{
			Curl& curl = Curl::get();

			// Check if response is cached already
			if (!request.force_)
			{
				auto cachedReponse = curl.responses_->get(request);
				if (cachedReponse)
					return *cachedReponse;
			}

			// Run new request
			Curl::Response response(request);
			Curl::get().processInternally(response);

			// Cache it
			if (response.code_ == Status::SUCCESS)
				curl.responses_->put(request, response);

			return response;
		}

		Curl::Method Curl::toMethod(std::string_view str)
		{
			std::string m{ str };
			boost::algorithm::to_upper(m);

			if		(m == "GET")
				return Curl::Method::GET;
			else if (m == "POST")
				return Curl::Method::POST;
			else
				return Curl::Method::INVALID;
		}

		void Curl::Response::extractInfo(CURL*&  curl)
		{
			// Results from curl_easy_getinfo calls are not checked

			assert(code_ == Status::SUCCESS);

			//curl_easy_getinfo(curl, CURLINFO_FILETIME, &date_); // TODO: impl
			//curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &duration_); // TODO: impl
			curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &bytes_);
			curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &speed_);
			curl_easy_getinfo(curl, CURLINFO_LOCAL_PORT, &localPort_);

			// Method verification
			char* method = nullptr;
			curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_METHOD, &method);		
			assert(method != nullptr);
			method_ = Curl::toMethod(method);
			ENSURE(this->method_ == request_.method_, "request and response method are different");

			// Even if at this point curl_easy_perform returned CURLE_OK, still need to check the actual http code returned
			int64_t httpCode = 400;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
			if (httpCode != 200)
				code_ = Status::CURL_REQUEST_FAILED;
		}

		void Curl::Response::print() const
		{
			
		}

		// TODO2: really should avoid init a new curl easy handle every single time
		void Curl::processInternally(Curl::Response& response)
		{
			Request& request = response.request_;

			// Curl must be initialized before running an Http request
			if (!initialized_)
			{
				response.code_ = Status::CURL_NOT_INITIALIZED;
				return;
			}

			// Initialize a CURL handle
			CURL* curl;
			curl = curl_easy_init(); // TODO: create a curl handle maker that works in a RAII fashion
			if (!curl)
			{
				response.code_ = Status::CURL_HANDLE_INIT_FAILED;
				return;
			}

			// Url
			response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_URL, request.url_.c_str()), Status::CURL_URL_SET_FAILED); RETURN_ON_INVALID_RESPONSE;

			// Callback
			response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback), Status::CURL_OPT_SET_FAILED); RETURN_ON_INVALID_RESPONSE;
			response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.data_), Status::CURL_WRITEDATA_SET_FAILED); RETURN_ON_INVALID_RESPONSE;

			// Set maximum time to complete the operation to 10 seconds
			response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L), Status::CURL_OPT_SET_FAILED); RETURN_ON_INVALID_RESPONSE;

			if (request.method_ == Method::POST)
			{
				// Specify POST request
				response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_POST, 1L), Status::CURL_POST_SET_FAILED); RETURN_ON_INVALID_RESPONSE;

				// Set header and data
				struct curl_slist* slist = NULL;
				slist = curl_slist_append(slist, "Content-Type: application/json");
				slist = curl_slist_append(slist, "Accept: application/json");
				response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist), Status::CURL_HEADER_SET_FAILED); RETURN_ON_INVALID_RESPONSE;
				response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.data_.c_str()), Status::CURL_DATA_SET_FAILED); RETURN_ON_INVALID_RESPONSE;
			}

			// Perform the request
			response.code_ = CURL_CALL(curl_easy_perform(curl), Status::CURL_PERFORM_FAILED); RETURN_ON_INVALID_RESPONSE;

			// Extract info from curl handle only when the request was successful
			response.extractInfo(curl);

			// Clean-up the CURL handle
			curl_easy_cleanup(curl);
		}
	}

}

