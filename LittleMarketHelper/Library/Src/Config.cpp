
#include "Config.h"


namespace lmh {

	Status Config::initialize(const std::string& filename)
	{
		Config& c = Config::get();

		// Check if already initialized
		if (c.json_.initialized())
			return Status::CONFIG_ALREADY_INITIALIZED;

		// Open file
		const std::ifstream stream(filename);
		if (!stream.is_open())
			return Status::FILE_NOT_OPEN;

		// try to parse it
		return c.json_.parse(filename);
	}

	const pt::ptree& Config::properties() const
	{
		REQUIRE(json_.initialized(), "config not initilized at startup");
		return json_.tree();
	}

}