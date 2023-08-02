// ========================================================================
//		Base parsers class
// ========================================================================

#pragma once

#include <filesystem>

#include "Assertions.h"

namespace fs = std::filesystem;


namespace lmh {

	class ReportParser
	{
	public:

		// Supported parsers
		enum class Type
		{
			DEGIRO
		};

	public:

		ReportParser(Type type, const fs::path& reportPath);
		virtual ~ReportParser() = default;

		// Non-const methods
		//virtual PortfolioTrades parse() const = 0;
		
		// Const methods
		inline void setFile(const fs::path& reportPath);	

	protected:

		fs::path reportPath_;
		Type type_;
	};


	// Inline definitions

	void ReportParser::setFile(const fs::path& reportPath)
	{
		REQUIRE(reportPath.has_filename(), "fullpath not provided, file name is missing");

		reportPath_ = reportPath;
	}

}