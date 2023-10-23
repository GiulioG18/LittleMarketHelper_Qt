// ========================================================================
//		DEGIRO parser class
// ========================================================================

#pragma once

#include "ReportParser.h"

#include "Utils/StatusCode.h"
#include "TickersGenerator.h"


namespace lmh {

	class DegiroReportParser : public ReportParser
	{
	public:

		DegiroReportParser() = default;
		virtual ~DegiroReportParser() = default;

		// Non-const methods
		// NB: it reads them from the config file, which MUST first be init when parsing with DEGIRO
		// NB2: Curl MUST be initialized before parsing with DEGIRO aswell in order to run requests
		virtual Status readFile(const fs::path& file, Output& output) const override;

		// Const methods
		virtual fs::path defaultFilename() const override;
		virtual fs::path defaultExtension() const override;
	};

}
