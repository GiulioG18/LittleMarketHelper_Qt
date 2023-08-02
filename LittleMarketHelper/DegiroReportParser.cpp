
#include "DegiroReportParser.h"


namespace lmh {

	DegiroReportParser::DegiroReportParser(ReportParser::Type type, const fs::path& reportPath)
		: ReportParser(type, reportPath)
	{
	}

	/*PortfolioTrades DegiroReportParser::parse() const
	{
		REQUIRE(!reportPath_.empty(), "path to report has not been set");
		REQUIRE(reportPath_.extension() == ".csv", "'.csv' extension expected for DEGIRO reports");

		PortfolioTrades trades;

		return trades;
	}*/

}