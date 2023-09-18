
// ========================================================================
//		Config file
// ========================================================================

#pragma once


#include <filesystem>
#include <string>
#include <optional>

#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"


namespace fs = std::filesystem;

namespace lmh {

	class Config : public Singleton<Config>
	{
		friend class Singleton<Config>;

	private:

		Config() = default;

	public:

		static LmhStatus initialize(const fs::path& config);

		// Path must contain all the nodes delimited by a dot (.) 
		// Last chunk of the path is the key of the target JSON object
		template<typename Type>
		std::optional<Type> get(const std::string& path) const;

		// Getters
		inline bool initialized() const;

	private:

		bool initialized_ = false;
	};


	// Inline definitions
	inline bool Config::initialized() const { return initialized_; }
}