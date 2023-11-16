
#include "Config.h"


namespace lmh {

	// TODO2: create path class to easily concatenate stuff and wrap the json get method

	Status Config::initialize(const fs::path& filename)
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

	const pt::ptree& Config::properties()
	{
		Config& c = Config::get();

		REQUIRE(c.json_.initialized(), "config not initilized at startup");
		return c.json_.tree();
	}

}