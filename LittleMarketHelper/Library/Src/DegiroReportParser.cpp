
#include "DegiroReportParser.h"


namespace lmh {

	DegiroReportParser::DegiroReportParser(ReportParser::Type type)
		: ReportParser(type)
	{
	}

	std::pair<std::vector<FinProduct>, bool> DegiroReportParser::run(std::vector<FinProduct>& products, bool& successful) const
	{
		REQUIRE(type_ == ReportParser::Type::DEGIRO, "parser type is not DEGIRO");
		REQUIRE(successful, "parser status is invalid");
		
	}

	fs::path DegiroReportParser::defaultFilename() const
	{
		return fs::path("Portfolio");
	}

	fs::path DegiroReportParser::defaultExtension() const
	{
		return fs::path(".csv");
	}

}