
// ========================================================================
//		Report parsers
// ========================================================================

#pragma once

#include <filesystem>
#include <deque>
#include <memory>
#include <utility>
#include <variant>

#include "Utils/Assertions.h"
#include "Utils/StatusCode.h"
#include "SecurityShell.h"
#include "Currency.h"
#include "MarketData/Security.h"


namespace fs = std::filesystem;

namespace lmh {

	// Available parsers

	enum class ParserType
	{
		UNDEFINED = 0,
		DEGIRO
	};



	// Base

	class ReportParser
	{
	public:
		
		struct Output;

	public:

		static Output parse(ParserType type, const fs::path& report = ""); // [ MAY THROW ]
		virtual ~ReportParser() = default;

	protected:

		ReportParser();

	protected:

		virtual Output extractSecurities(std::ifstream& stream) const = 0;
		virtual fs::path defaultFilename() const;
		std::string parseName(const std::string& section, bool& error) const;
		std::string parseIsin(const std::string& section, bool& error) const;
		uint32_t parseQuantity(const std::string& section, bool& error) const;
		Currency parseCurrency(const std::string& section, bool& error) const;


	private:

		// Factory method
		static std::unique_ptr<ReportParser> create(ParserType type);
		// Returns an empty path if no valid report file is found
		fs::path findReport();

	protected:

		// TODO: add mds access here
		fs::path report_;
		std::deque<fs::path> guesses_;
	};


	struct ReportParser::Output
	{
		using Securities = std::vector<std::variant<Security, SecurityShell>>;

		Status status_				= Status::REPORT_NOT_FOUND;
		ParserType type_			= ParserType::UNDEFINED;
		Securities securities_;
	};



	// DEGIRO

	class DegiroReportParser : public ReportParser
	{
	public:

		DegiroReportParser() = default;
		virtual ~DegiroReportParser() = default;

		virtual Output extractSecurities(std::ifstream& stream) const override;
		virtual fs::path defaultFilename() const override;
	};

}