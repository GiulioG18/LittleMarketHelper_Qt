
#include "ReportParser.h"
#include "DegiroReportParser.h"


namespace lmh {

	ReportParser::ReportParser(ReportParser::Type type)
		: type_(type)
	{
		// TODO: what about other OS executables?
		fs::path download = std::string(std::getenv("USERPROFILE")) + '\\' + "Downloads";
		fs::path current = fs::current_path();

		if (fs::is_directory(current))
		{
			guesses_.insert(current);
		}

		if (fs::is_directory(download))
		{
			guesses_.insert(download);
		}
	}

	std::pair<std::vector<FinProduct>, bool> ReportParser::parse(std::optional<fs::path> file) const
	{
		std::vector<FinProduct> products;
		bool successful = true;

		if (!file)
		{
			auto [products, successful] = parseDefault();
		}

		if (!successful)
		{			

			// Check that the file exists and is readable, if so, parse it
			
		}

		return { products, successful };
	}

	std::pair<std::vector<FinProduct>, bool> ReportParser::parseDefault() const
	{
		std::unique_ptr<ReportParser> parser = create(type_);
		ASSERT(parser, "invalid parser");

		std::vector<FinProduct> products;
		bool successful = true;

		fs::path file = parser->defaultFilename();
		file += parser->defaultExtension();	// Add file default extension to file name
		if (!file.has_filename() || !file.has_extension())
		{
			// No default file, return
			successful = false;
		}
		else
		{
			// Default file is provided for this parser, try 
			// to guess the directory and, if found, parse it
			//parser->run(products, successful);
		}

		return { products, successful };
	}

	std::unique_ptr<ReportParser> ReportParser::create(ReportParser::Type type)
	{
		switch (type)
		{
		case ReportParser::Type::DEGIRO: return std::make_unique<DegiroReportParser>(type); break;

		default: ASSERT(false, "unknown report type"); break;
		}
	}

	fs::path ReportParser::defaultFilename() const
	{
		return fs::path("");
	}

	fs::path ReportParser::defaultExtension() const
	{
		return fs::path("");
	}

}