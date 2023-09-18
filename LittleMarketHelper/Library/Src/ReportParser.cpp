
#include "ReportParser.h"
#include "DegiroReportParser.h"


namespace lmh {

	ReportParser::ReportParser()
	{
		// Initialize guesses dirs
		// TODO: what about other OS executables?
		fs::path downloads = std::string(std::getenv("USERPROFILE")) + '\\' + "Downloads";
		fs::path documents = std::string(std::getenv("USERPROFILE")) + '\\' + "Documents";

		if (fs::is_directory(downloads))
			guesses_.insert(downloads);

		if (fs::is_directory(documents))
			guesses_.insert(documents);
	}

	ReportParser::Output ReportParser::parseDefault(ReportParser::Type type)
	{
		ReportParser::Output output(type);

		std::unique_ptr<ReportParser> parser = create(type);
		ASSERT(parser, "invalid parser");

		fs::path file = parser->defaultFilename();
		file += parser->defaultExtension();			// Add file default extension to file name
		if (file.has_filename())
		{
			// Default file is provided for this parser. Try 
			// to guess its folder and, if found, parse it
			// NB: this is not a recursive search, could make it so but  
			//	   i'm unsure of how it would go with privileges and stuff
			//	   (only few pre-defined user folders are searched)
			for (const auto& folder : parser->guesses_)
			{
				ASSERT(fs::is_directory(folder), "not a valid folder");

				fs::path absolutePath;
				absolutePath += folder;
				absolutePath += "\\";
				absolutePath += file;

				if (fs::is_regular_file(absolutePath))
				{
					output.statusLmhStatus_ = parser->readFile(absolutePath, output);
					if (output.statusLmhStatus_ == LmhStatus::SUCCESS)
						break;
				}
			}
		}

		return output;
	}

	ReportParser::Output ReportParser::parse(ReportParser::Type type, const fs::path& report)
	{
		ReportParser::Output output(type);

		// Initialize parser
		std::unique_ptr<ReportParser> parser = create(type);
		ASSERT(parser, "invalid parser");

		if (fs::is_regular_file(report))
		{
			output.statusLmhStatus_ = parser->readFile(report, output);
		}
		
		return output;
	}

	std::unique_ptr<ReportParser> ReportParser::create(ReportParser::Type type)
	{
		switch (type)
		{
		case ReportParser::Type::DEGIRO: return std::make_unique<DegiroReportParser>();			break;

		default: ASSERT(false, "unknown report type");											break;
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