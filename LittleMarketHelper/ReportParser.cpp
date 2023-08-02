
#include "ReportParser.h"


namespace lmh {

	ReportParser::ReportParser(Type type, const fs::path& reportPath)
		: type_(type)
	{
		setFile(reportPath);
	}

}