
#include "Utils/Warnings.h"

#include <boost/algorithm/string.hpp>
#include <algorithm>

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
		return;											\
	}													\



namespace lmh {

	namespace http {

		// Response cache size
		static constexpr size_t MAX_CACHED_REQUESTS = 20;


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
			curl.responses_ = std::make_unique<FifoCache<Request, Response>>(MAX_CACHED_REQUESTS);

			// Finalize
			curl.initialized_ = true;
			return Status::SUCCESS;
		}

		Curl::Response Curl::httpRequest(const Curl::Request& request)
		{
			Curl& curl = Curl::get();

			// Check if response is cached already
			if (!request.skipCache_)
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

		Curl::Method Curl::stom(std::string_view str)
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

		std::string Curl::mtos(Curl::Method method)
		{
			switch (method)
			{
			case lmh::http::Curl::Method::INVALID:	return std::string("INVALID"); 
			case lmh::http::Curl::Method::GET:		return std::string("GET"); 
			case lmh::http::Curl::Method::POST:		return std::string("POST"); 

			default:								FAIL("unknown method");								
			}
		}

		// NB: status code from all curl_easy_getinfo calls is ignored
		void Curl::Response::extractInfo(CURL*&  curl)
		{
			assert(code_ == Status::SUCCESS);

			// Extract date
			int64_t sinceEpoch = 0;
			curl_easy_getinfo(curl, CURLINFO_FILETIME, &sinceEpoch); // TODO: this time is really off, check why...
			date_ = Date(Date::UnixTimestamp(std::max(sinceEpoch, 0LL))); 

			// Extract duration
			double duration = 0.0;
			curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &duration);

			duration_ = Milliseconds{ static_cast<int64_t>(duration * 1000) };

			// Extract rest
			curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &bytes_);
			curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &speed_);
			curl_easy_getinfo(curl, CURLINFO_LOCAL_PORT, &localPort_);

			// Method verification
			char* method = nullptr;
			curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_METHOD, &method);		
			assert(method != nullptr);
			ENSURE(stom(method) == request_.method_, "method verification failed");

			// Even if at this point curl_easy_perform returned CURLE_OK, still need to check the actual http code returned
			int64_t httpCode = 400;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
			if (httpCode != 200)
				code_ = Status::CURL_REQUEST_FAILED;
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

			// Get a CURL handle
			HandleMaker hm;
			auto curl = hm.get();
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

			// Ask for filetime
			response.code_ = CURL_CALL(curl_easy_setopt(curl, CURLOPT_FILETIME, 1L), Status::CURL_OPT_SET_FAILED); RETURN_ON_INVALID_RESPONSE;

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
		}




		// Curl::Request

		Curl::Request::Request(Method method, std::string_view url, std::string_view data, bool skipCache)
			:
			method_(method),
			url_(url),
			data_(data),
			skipCache_(skipCache)
		{
			// Verifications
			ENSURE(!url_.empty(), "empty url not allowed");
			ENSURE(method_ != Method::INVALID, "invalid method");
			if (method_ == Method::GET)
				ENSURE(data_.empty(), "data must be empty for GET requests");
		}

		constexpr auto Curl::Request::key() const
		{
			return std::tie(method_, url_, data_);
		}

		auto Curl::Request::operator<=>(const Request& other) const
		{
			return this->key() <=> other.key();
		}

		bool Curl::Request::operator==(const Request& other) const
		{
			return this->key() == other.key();
		}




		// Curl::Response

		Curl::Response::Response(const Request& request)
			:
			request_(request),
			id_(0),
			code_(Status::SUCCESS),
			duration_(0),
			bytes_(0),
			speed_(-1.0),
			localPort_(0)
		{
			// Initialize ID
			static uint64_t id = 0;
			id_ = id++;
		};

		void Curl::Response::print(std::ostream& os) const
		{
			os << "==========================================" << "\n";
			os << "HTTP request n: " << id_ << "\n";
			os << "Method: " << mtos(request_.method_) << "\n";
			os << "URL: " << request_.url_ << "\n";
			os << "Header: " << request_.data_ << "\n";
			os << "Skip cache: " << std::boolalpha << request_.skipCache_ << "\n";
			os << "Code: " << error::stos(code_) << "\n";
			os << "Date: " << date_ << "\n";
			os << "Total duration (s): " << duration_ << "\n";
			os << "Bytes downloaded: " << bytes_ << "\n";
			os << "Speed (bytes/s): " << speed_ << "\n";
			os << "Local port used: " << localPort_ << "\n";
			os << "==========================================" << "\n";
		}





		// HandleMaker

		HandleMaker::HandleMaker()
			: handle_(curl_easy_init())
		{

		}

		HandleMaker::~HandleMaker()
		{
			curl_easy_cleanup(handle_);
		}

		inline CURL* const HandleMaker::get()
		{
			return handle_;
		}

	}

}

