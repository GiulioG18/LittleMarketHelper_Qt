
// ========================================================================
//		Utility to handle JSON files		
// ========================================================================

#pragma once

#include <filesystem>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Assertions.h"
#include "Utils/StatusCode.h"


namespace pt = boost::property_tree;
namespace fs = std::filesystem;

namespace lmh {

	class Json
	{
	public:

		Json() = default;

		Status parse(const std::string& filename);
		Status parse(std::stringstream& ss);
		bool initialized() const;
		void clear();

		// Getters
		inline const pt::ptree& tree() const; // [ MAY THROW ]
		inline const fs::path& filename() const; // [ MAY THROW ]

	private:

		std::string filename_;
		pt::ptree tree_;
	};


	// Inline definitions
	inline const pt::ptree& Json::tree() const { REQUIRE(initialized(), "Json file not initialized"); return tree_; };
	inline const fs::path& Json::filename() const { REQUIRE(initialized(), "Json file not initialized"); return filename_; };

}
