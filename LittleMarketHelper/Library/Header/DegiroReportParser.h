// ========================================================================
//		DEGIRO parser class
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
		virtual void readFile(const fs::path& file, std::vector<FinProduct>& products, bool& successful) const override;
		virtual fs::path defaultFilename() const override;
		virtual fs::path defaultExtension() const override;
	};

}
