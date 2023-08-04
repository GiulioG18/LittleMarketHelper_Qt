// ========================================================================
//		DEGIRO parser class
// ========================================================================

#pragma once

#include <filesystem>

#include "Utils/Assertions.h"


namespace fs = std::filesystem;

namespace lmh {

	class File
	{
	public:
		
		static inline bool readable(const fs::path& file);
		static inline bool writable(const fs::path& file);
	};

	inline bool lmh::File::readable(const fs::path& file)
	{
		bool status = true;

		// It exists
		status = fs::exists(file);
		if (!status) return;
		// There is a file in the path
		status = file.has_filename();
		if (!status) return;

		auto permissions = fs::status(file).permissions();
		if ((permissions & fs::perms::owner_read) == fs::perms::none ||
			(permissions & fs::perms::group_read) == fs::perms::none ||
			(permissions & fs::perms::others_read) == fs::perms::none
			)
		{
			status = false;
		}		

		return status;
	}

	// TODO: change the logic, this should work with folders.
	//		 Ideally when this returns true, subsequent tries to open() should work fine
	inline bool lmh::File::writable(const fs::path& folder)
	{
		bool status = true;

		// It exists
		status = fs::exists(folder);
		if (!status) return;
		// There is a file in the path
		status = !file.has_filename();
		if (!status) return;
				
		auto permissions = fs::status(file).permissions();
		if ((permissions & fs::perms::owner_write) == fs::perms::none ||
			(permissions & fs::perms::group_write) == fs::perms::none ||
			(permissions & fs::perms::others_write) == fs::perms::none
			)
		{
			status = false;
		}

		// TODO: check if writable
		return status;
	}

}
