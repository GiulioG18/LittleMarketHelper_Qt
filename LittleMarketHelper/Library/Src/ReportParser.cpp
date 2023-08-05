
#include "ReportParser.h"
#include "DegiroReportParser.h"


namespace lmh {

	ReportParser::ReportParser(ReportParser::Type type)
		: type_(type)
	{
		// TODO: what about other OS executables?
		fs::path downloads = std::string(std::getenv("USERPROFILE")) + '\\' + "Downloads";
		fs::path documents = std::string(std::getenv("USERPROFILE")) + '\\' + "Documents";

		if (fs::is_directory(downloads))
		{
			guesses_.insert(downloads);
		}

		if (fs::is_directory(documents))
		{
			guesses_.insert(documents);
		}
	}

	std::pair<std::vector<FinProduct>, bool> ReportParser::parse(ReportParser::Type type, std::optional<fs::path> file)
	{
		std::vector<FinProduct> products;
		bool successful = true;

		if (!file.has_value())
		{
			auto [products, successful] = parseDefault(type);
		}

		if (file.has_value() || !successful)
		{			
			std::unique_ptr<ReportParser> parser = create(type);
			ASSERT(parser, "invalid parser");

			if (fs::is_regular_file(file.value()))
			{
				parser->readFile(file.value(), products, successful);
			}			
		}

		return { products, successful };
	}

	std::pair<std::vector<FinProduct>, bool> ReportParser::parseDefault(ReportParser::Type type)
	{
		std::unique_ptr<ReportParser> parser = create(type);
		ASSERT(parser, "invalid parser");

		std::vector<FinProduct> products;
		bool successful = true;

		fs::path file = parser->defaultFilename();
		file += parser->defaultExtension();	// Add file default extension to file name
		if (!file.has_filename())
		{
			// No default file, return
			successful = false;
		}
		else
		{
			// Default file is provided for this parser. Try 
			// to guess its folder and, if found, parse it
			// NB: for performance reason, this only checks
			//	   few pre-defined user folders
			for (const auto& folder : parser->guesses_)
			{
				ASSERT(fs::is_directory(folder), "not a valid folder");

				fs::path absolutePath;
				absolutePath += folder;
				absolutePath += "\\";
				absolutePath += file;

				if (fs::is_regular_file(absolutePath))
				{
					parser->readFile(absolutePath, products, successful);
					if (successful) 
						break;
				}
			}
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