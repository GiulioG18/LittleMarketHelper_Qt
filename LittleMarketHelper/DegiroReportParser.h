// ========================================================================
//		Base parsers class
// ========================================================================

#pragma once

#include "ReportParser.h"


namespace lmh {

	class DegiroReportParser : public ReportParser
	{
	public:

		DegiroReportParser(ReportParser::Type type, const fs::path & reportPath);
		virtual ~DegiroReportParser() = default;

		//virtual PortfolioTrades parse() const override;
	};

}
