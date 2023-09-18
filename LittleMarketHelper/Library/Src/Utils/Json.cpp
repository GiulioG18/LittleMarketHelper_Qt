
#include "Utils/Json.h"


namespace lmh {

	LmhStatus Json::parse(const std::string& filename)
	{
		try
		{
			pt::read_json(filename, tree_);
			if (tree_.empty())
				throw;
		}
		catch (...)
		{
			return LmhStatus::INVALID_JSON_FORMAT;
		}

		// Assign filename
		filename_ = filename;

		return LmhStatus::SUCCESS;
	}

	LmhStatus Json::parse(std::stringstream& ss)
	{
		try
		{
			pt::read_json(ss, tree_);
			if (tree_.empty())
				throw;
		}
		catch (...)
		{
			return LmhStatus::INVALID_JSON_FORMAT;
		}

		// No filename
		filename_ = "";

		return LmhStatus::SUCCESS;
	}

	bool Json::initialized() const
	{
		return !tree_.empty();
	}

}