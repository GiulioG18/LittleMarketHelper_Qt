
#include "Utils/Json.h"
#include "Utils/Assertions.h"


namespace lmh {

	Status Json::parse(const std::string& filename)
	{
		try
		{
			pt::read_json(filename, tree_);
			if (tree_.empty())
				FAIL("empty tree");
		}
		catch (...)
		{
			return Status::INVALID_JSON_FORMAT;
		}

		// Assign filename
		filename_ = filename;

		return Status::SUCCESS;
	}

	Status Json::parse(std::stringstream& ss)
	{
		try
		{
			pt::read_json(ss, tree_);
			if (tree_.empty())
				throw;
		}
		catch (...)
		{
			return Status::INVALID_JSON_FORMAT;
		}

		// No filename
		filename_ = "";

		return Status::SUCCESS;
	}

	bool Json::initialized() const
	{
		return !tree_.empty();
	}

	void Json::clear()
	{
		filename_.clear();
		tree_.clear();
	}

}