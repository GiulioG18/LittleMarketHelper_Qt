
#include "Http/Api.h"


namespace lmh {

	namespace http {


		// Base

		Status Api::send()
		{
			Curl& curl = Curl::get();

			// Reset stats
			lastStats_ = std::make_unique<Stats>();

			// Send request
			Status status;
			if (method_ == "get")
				status = sendGetRequest();
			else if (method_ == "post")
				status = sendPostRequest();
			else
				status = Status::API_INVALID_METHOD;

			if (status != Status::SUCCESS)
				status;

			// Write stats
			// TODO: impl

			// Write response
			response_ = curl.response();
			return Status::SUCCESS;
		}

		/*Api::Api(
			const std::string& name,
			const std::string& method,
			const std::string& url,
			const std::string& data,
			const std::string& filler,
			std::set<std::string>&& keys
		)
			:
			name_(name),
			method_(method),
			url_(url),
			data_(data),
			filler_(filler),
			keys_(keys),
			response_(""),
			json_(Json()),
			lastStats_(nullptr)
		{
		}*/
											
		Status Api::writeJson()
		{
			std::stringstream ss;
			ss << response_;
			return json_.parse(ss);
		}

		Status Api::sendGetRequest()
		{
			Status status = Status::SUCCESS;

			// Validate input
			REQUIRE(!url_.empty(), "empty url");
			REQUIRE(!filler_.empty(), "empty filler for GET request");

			replacePlaceholder(url_, filler_);
			return Curl::get().GETRequest(url_);
		}

		Status Api::sendPostRequest()
		{
			Status status = Status::SUCCESS;

			// Validate input
			REQUIRE(!url_.empty(), "empty url");
			REQUIRE(!filler_.empty(), "empty filler for POST request");
			REQUIRE(!data_.empty(), "empty data for POST request");

			replacePlaceholder(data_, filler_);
			return Curl::get().POSTRequest(url_, data_);
		}

		void Api::replacePlaceholder(std::string& s, const std::string& value) const
		{
			auto pos = s.find("%%%PLACEHOLDER%%%");
			if (pos == std::string::npos)
				return;

			// Replace it (%%%PLACEHOLDER%%% is 17 letters)
			s.replace(pos, 17, value);
		}


		// Api stats

		void Api::Stats::clear()
		{
			bytes_ = static_cast<size_t>(0);
			time_ = std::chrono::milliseconds::zero();
		}


		// Quote

		Quote::Quote()
		{
			std::string path = "httpRequest.quoteByFullTicker.yahooFinance";
		}

	}
}