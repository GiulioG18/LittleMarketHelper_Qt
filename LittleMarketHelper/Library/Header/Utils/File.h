// ========================================================================
//		DEGIRO parser class
// ========================================================================

#pragma once

#include <filesystem>
#include <fstream>

#include "Utils/StatusCode.h"
#include "Utils/Assertions.h"


namespace fs = std::filesystem;

namespace lmh {

	class File
	{
	public:
		
		static inline bool readable(const fs::path& file);
		static inline bool writable(const fs::path& file);
	};


	// Inline definitions
	inline bool lmh::File::readable(const fs::path& file)
	{
		bool status = true;

		// It exists
		status = fs::exists(file);
		if (!status) 
			return status;

		// ...and it's a file
		status = fs::is_regular_file(file);
		if (!status) 
			return status;

		auto permissions = fs::status(file).permissions();
		if (
			(permissions & fs::perms::owner_read) == fs::perms::none ||
			(permissions & fs::perms::group_read) == fs::perms::none ||
			(permissions & fs::perms::others_read) == fs::perms::none
			)
		{
			status = false;
		}

		return status;
	}

	inline bool lmh::File::writable(const fs::path& folder)
	{
		bool status = true;

		// It exists
		status = fs::exists(folder);
		if (!status) 
			return status;

		// ...and it's a folder
		status = fs::is_directory(folder);
		if (!status) 
			return status;
				
		auto permissions = fs::status(folder).permissions();
		if ((permissions & fs::perms::owner_write) == fs::perms::none ||
			(permissions & fs::perms::group_write) == fs::perms::none ||
			(permissions & fs::perms::others_write) == fs::perms::none
			)
		{
			status = false;
		}

		return status;
	}

}
