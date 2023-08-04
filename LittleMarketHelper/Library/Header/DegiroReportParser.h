// ========================================================================
//		Base parsers class
// ========================================================================

#pragma once

#include "ReportParser.h"


namespace lmh {

	class DegiroReportParser : public ReportParser
	{
	public:

		DegiroReportParser(ReportParser::Type type);
		virtual ~DegiroReportParser() = default;

		// Const methods
		virtual std::pair<std::vector<FinProduct>, bool> run(std::vector<FinProduct>& products, bool& successful) const override;
		virtual fs::path defaultFilename() const override;
		virtual fs::path defaultExtension() const override;
	};

}
