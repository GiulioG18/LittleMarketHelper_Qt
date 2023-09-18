// ========================================================================
//		Base parsers class
// ========================================================================

#pragma once

#include <filesystem>
#include <set>
#include <memory>
#include <utility>
#include <variant>

#include "Utils/Assertions.h"
#include "Security.h"
#include "Utils/StatusCode.h"


namespace fs = std::filesystem;

namespace lmh {


	class ReportParser
	{
	public:

		//										Isin		Name
		using UncompleteSecurity = std::pair<std::string, std::string>;
		using ParsedSecurities = std::vector<std::variant<Security, ReportParser::UncompleteSecurity>>;
		
		struct Output;

		// Supported parsers
		enum class Type;

	public:

		static Output parseDefault(ReportParser::Type type);			// Reads the first report found
		static Output parse(ReportParser::Type type, const fs::path& report);
		virtual ~ReportParser() = default;

	protected:

		ReportParser();

	protected:

		// Non-const methods
		virtual LmhStatus readFile(const fs::path& file, Output& output) const = 0;

		// Const methods
		virtual fs::path defaultFilename() const;
		virtual fs::path defaultExtension() const;

	private:

		// Factory method
		static std::unique_ptr<ReportParser> create(ReportParser::Type type);

	protected:

		std::set<fs::path> guesses_;
	};


	enum class ReportParser::Type
	{
		DEGIRO
	};


	struct ReportParser::Output
	{
		ReportParser::Output(ReportParser::Type type)
			: 
			type_(type), 
			found_(0),
			discarded_(0),
			statusLmhStatus_(LmhStatus::REPORT_NOT_FOUND)
		{
		}

		ReportParser::Type type_;
		ReportParser::ParsedSecurities parsedSecurities_;
		int found_;
		int discarded_;
		LmhStatus statusLmhStatus_;
	};

}