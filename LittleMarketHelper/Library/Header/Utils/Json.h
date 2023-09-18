
// ========================================================================
//		Utility to handle JSON files		
// ========================================================================

#pragma once

#include <filesystem>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Utils/StatusCode.h"


namespace pt = boost::property_tree;
namespace fs = std::filesystem;

namespace lmh {

	class Json
	{
	public:

		Json() = default;

		LmhStatus parse(const std::string& filename);
		LmhStatus parse(std::stringstream& ss);
		bool initialized() const;

		// Getters
		inline const pt::ptree& tree() const;
		inline const fs::path& filename() const;

	private:

		std::string filename_;
		pt::ptree tree_;
	};


	// Inline definitions
	inline const pt::ptree& Json::tree() const { return tree_; };
	inline const fs::path& Json::filename() const { return filename_; };

}
