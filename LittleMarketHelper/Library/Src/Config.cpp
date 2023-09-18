
#include <boost/json.hpp>

#include <fstream>
#include <vector>

#include "Config.h"
#include "Utils/Assertions.h"


namespace bj = boost::json;			// :)

namespace lmh {

	// Global JSON 
	// NB: outside of class to avoid including boost.JSON to other translation units
	bj::value _config;

	// Helper to retrieve a nested value inside the config file
	// NB: does not support big integers
	template<typename Type>
	std::optional<Type> getNestedValue(const std::string& path)
	{
		std::vector<std::string> nodes;
		std::string key;

		// Split path (I really don't want to use a property_tree)
		std::string delimiter = ".";
		size_t last = 0;
		size_t next = 0;
		while ((next = path.find(delimiter, last)) != std::string::npos)
		{
			nodes.emplace_back(path.substr(last, next - last));
			last = next + 1;
		}

		// Last chunk of the path provided is the key of the JSON object
		key = path.substr(last);

		// Search into config file
		bj::object v = _config.as_object();
		bool valid = true;
		for (size_t nodeId = 0; nodeId < nodes.size(); nodeId++)
		{
			auto it = v.find(nodes[nodeId]);
			// Intermediate node not found or does not contain an object
			if (it == v.end() || !it->value().is_object())
			{
				valid = false;
				break;
			}

			v = it->value().as_object();
		}

		if (!valid)
			return std::nullopt;

		// Check value type
		if constexpr (std::is_same_v<Type, bool>)
		{
			if (v.find(key)->value().if_bool())
				return std::make_optional(static_cast<bool>(v.find(key)->value().as_bool()));
		}
		else if constexpr (std::is_same_v<Type, int>)
		{
			if (v.find(key)->value().if_int64())
				return std::make_optional(static_cast<int>(v.find(key)->value().as_int64()));
		}
		else if constexpr (std::is_same_v<Type, float>)
		{
			if (v.find(key)->value().if_double()) // Decimal numbers in the JSON library are always double
				return std::make_optional(static_cast<float>(v.find(key)->value().as_double()));
		}
		else if constexpr (std::is_same_v<Type, std::string>)
		{
			if (v.find(key)->value().if_string())
				return std::make_optional(std::string(v.find(key)->value().as_string()));
		}
		else
		{
			ASSERT(false, "invalid JSON type");
		}

		// If key was not valid, return nullopt
		return std::nullopt;		
	}

	LmhStatus Config::initialize(const fs::path& configFile)
	{
		LmhStatus status = LmhStatus::SUCCESS;
		Config& config = Config::instance();

		if (config.initialized_)
			return LmhStatus::CONFIG_ALREADY_INITIALIZED;

		// Open file
		const std::ifstream stream{ configFile };
		if (!stream.is_open())
			return LmhStatus::FILE_NOT_OPEN;

		// Parse it
		std::stringstream buffer;
		buffer << stream.rdbuf();
		try
		{
			_config = bj::parse(buffer.str());
		}
		catch (...)
		{
			status = LmhStatus::INVALID_JSON_FORMAT;
		}
		if (status != LmhStatus::SUCCESS)
			return status;

		// JSON must be an object
		if (!_config.is_object())
			return LmhStatus::INVALID_JSON_FORMAT;

		// Finalize
		config.initialized_ = true;
		return status;
	}


	// Template instantiations only for valid JSON types

	template<> std::optional<bool> Config::get<bool>(const std::string& path) const
	{
		if (!initialized_ || path.empty())
			return std::nullopt;

		return getNestedValue<bool>(path);
	}

	template<> std::optional<int> Config::get<int>(const std::string& path) const
	{
		if (!initialized_ || path.empty())
			return std::nullopt;

		return getNestedValue<int>(path);
	}

	template<> std::optional<float> Config::get(const std::string& path) const
	{
		if (!initialized_ || path.empty())
			return std::nullopt;

		return getNestedValue<float>(path);
	}

	template<> std::optional<std::string> Config::get(const std::string& path) const
	{
		if (!initialized_ || path.empty())
			return std::nullopt;

		return getNestedValue<std::string>(path);
	}

}