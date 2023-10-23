
#include "TickersGenerator.h"
#include "Utils/Assertions.h"


namespace lmh {

	Status Figi::generateTickers()
	{
		if (!json_.initialized())
			return Status::INVALID_JSON_FORMAT;

		// TODO:...

		return Status();
	}

	Status Figi::parse(const std::string& response)
	{
		Status status = Status::SUCCESS;

		// Clear response member
		response_.clear();

		// FIGI responses wrap the whole text in square brackets
		// Check if response is at least 2 chars long (squared brackets included)
		if (response.size() < 2)
			return Status::INVALID_INPUT;
		
		// - 2 for the brackets wrapping the text
		response_ = response.substr(1, response.size() - 2);

		std::stringstream ss;
		ss << response_;

		// Try parse it
		return json_.parse(ss);
	}

}