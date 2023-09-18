// ========================================================================
//		DEGIRO parser class
// ========================================================================

#pragma once

#include "ReportParser.h"

#include "Utils/StatusCode.h"


namespace lmh {

	class DegiroReportParser : public ReportParser
	{
	public:

		// TODO: degiro report does not report a valid (current) price.
		//		It does not even report a ticker. So this means we
		//		need to: first request a list of tickers with the figi api,
		//		and then request the current market price.
		//      - add urls and keys as members, initialize them in the ctor
		//		  (NB: it reads them from the config file, which MUST first be init when parsing with DEGIRO)
		//		  (NB2: Curl MUST be initialized before parsing with DEGIRO aswell in order to run requests!)
		//		- add FigiAPI as member to be reused for all prices

		DegiroReportParser() = default;
		virtual ~DegiroReportParser() = default;

		// Non-const methods
		virtual LmhStatus readFile(const fs::path& file, Output& output) const override;

		// Const methods
		virtual fs::path defaultFilename() const override;
		virtual fs::path defaultExtension() const override;
	};

}
