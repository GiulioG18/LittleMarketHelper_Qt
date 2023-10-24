
// ========================================================================
//		Config file
// ========================================================================

#pragma once


#include <string>

#include "Utils/Assertions.h"
#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "Utils/Json.h"


namespace pt = boost::property_tree;
namespace fs = std::filesystem;

namespace lmh {


	// Configuration file for LMH
	// NB: MUST be initialized (only once) at application start!!

	class Config : public Singleton<Config>
	{
		friend class Singleton<Config>;

	private:

		Config() = default;

	public:

		inline static Status initialize(const std::string& filename);

		// Assumes a const config file (it will throw if read value is not convertible into Type)
		template<typename Type>	static Type read(const std::string& path);
		static auto getChild(const std::string& path);
		

	private:

		Json json_;
	};


	// Inline definitions

	inline Status Config::initialize(const std::string& filename)
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

	inline auto Config::getChild(const std::string& path)
	{
		return Config::get().json_.tree().get_child(path);
	}


	// Template definitions
	template<typename Type>
	inline Type Config::read(const std::string& path)
	{
		Config& c = Config::get();

		REQUIRE(c.json_.initialized(), "config file not initialized");

		try
		{
			return c.json_.tree().get<Type>(path);
		}
		catch (...)
		{
			FAIL("can't read config file value");
		}
	}

}