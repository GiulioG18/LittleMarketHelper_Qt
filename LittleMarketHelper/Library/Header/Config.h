
// ========================================================================
//		Config file
// ========================================================================

#pragma once


#include <string>

#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "Utils/Json.h"


namespace pt = boost::property_tree;
namespace fs = std::filesystem;

namespace lmh {

	class Config : public Singleton<Config>
	{
		friend class Singleton<Config>;

	private:

		Config() = default;

	public:

		inline static LmhStatus initialize(const std::string& filename);
		inline bool initialized() const;

		template<typename Type>
		std::optional<Type> read(const std::string& path) const;

	private:

		Json json_;
	};


	// Inline definitions

	inline bool Config::initialized() const { return json_.initialized(); }

	inline LmhStatus Config::initialize(const std::string& filename)
	{
		Config& c = Config::instance();

		// Check if already initialized
		if (c.initialized())
			return LmhStatus::CONFIG_ALREADY_INITIALIZED;

		// Open file
		const std::ifstream stream(filename);
		if (!stream.is_open())
			return LmhStatus::FILE_NOT_OPEN;

		// try to parse it
		return c.json_.parse(filename);
	}


	// Template definitions

	template<typename Type>
	inline std::optional<Type> Config::read(const std::string& path) const
	{
		try
		{
			Type value = json_.tree().get<Type>(path);
			return std::make_optional(value);
		}
		catch (...)
		{
			return std::nullopt;
		}
	}

}