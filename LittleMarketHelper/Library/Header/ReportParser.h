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
		using UncompleteSecurity = std::pair<std::string, std::string>;	// TODO: make it a class with proper methods (could also avoid variant with polymorphism)
		using ParsedSecurities = std::vector<std::variant<Security, ReportParser::UncompleteSecurity>>;
		
		struct Output;

		// Supported parsers
		enum class Type;

	public:

		// The status of the report is written in the output
		static Output parseDefault(ReportParser::Type type); 
		static Output parse(ReportParser::Type type, const fs::path& report);
		virtual ~ReportParser() = default;

	protected:

		ReportParser();

	protected:

		// Non-const methods
		virtual Status readFile(const fs::path& file, Output& output) const = 0;

		// Const methods
		virtual fs::path defaultFilename() const;	// TODO: read from config file instead of hardcoding it (and then remove virtualness)
		virtual fs::path defaultExtension() const;	// TODO: read from config file instead of hardcoding it (and then remove virtualness)

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
			status_(Status::REPORT_NOT_FOUND)
		{
		}

		ReportParser::Type type_;
		ReportParser::ParsedSecurities parsedSecurities_;
		int found_;
		int discarded_;
		Status status_;
	};

}