
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

	class Config : public Singleton<Config>
	{
		friend class Singleton<Config>;

	private:

		Config() = default;

	public:

		static Status initialize(const std::string& filename);

		// Read only access to the Config ptree
		const pt::ptree& properties() const;

	private:

		Json json_;
	};

}