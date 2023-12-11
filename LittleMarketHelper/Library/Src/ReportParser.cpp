
#include <fstream>
#include <cassert>

#include "ReportParser.h"
#include "Utils/InputValidator.h"
#include "Http/Api.h"
#include "Config.h"


namespace lmh {

	// Base

	ReportParser::ReportParser()
	{
		// Initialize guesses dirs
		// TODO2: what about other OS executables?
		fs::path downloads = fs::path(std::getenv("USERPROFILE")) / "Downloads";
		fs::path documents = fs::path(std::getenv("USERPROFILE")) / "Documents";

		if (fs::is_directory(downloads))
			guesses_.push_back(downloads);

		if (fs::is_directory(documents))
			guesses_.push_back(documents);
	}

	ReportParser::Output ReportParser::parse(ParserType type, const fs::path& report)
	{
		std::unique_ptr<ReportParser> parser = create(type);
		assert(parser);

		// Guess the location of the report if no path is provided
		parser->report_ = report.empty() ? parser->findReport() : report;

		// Check if report path is not empty
		if (!parser->report_.empty())
		{
			// Try create an input file stream
			std::ifstream stream{ parser->report_ };

			// Check if is readable and non-empty
			if (stream.is_open() && stream.peek() != std::ifstream::traits_type::eof())
				return parser->extractSecurities(stream);
		}
			
		return ReportParser::Output{};

	}

	std::unique_ptr<ReportParser> ReportParser::create(ParserType type)
	{
		switch (type)
		{
		case ParserType::DEGIRO: return std::make_unique<DegiroReportParser>();		break;

		default: ASSERT(false, "invalid report type");								break;
		}
	}

	fs::path ReportParser::findReport()
	{		
		fs::path file = defaultFilename();

		if (!file.empty() && file.has_filename())
		{
			for (const auto& folder : guesses_) // This is not a recursive search, only few pre-selected folders are checked
			{
				if (!fs::is_directory(folder))
					continue;

				fs::path absolute(folder / file);
				if (fs::is_regular_file(absolute)) // TODO2: should check for read permissions too
					return absolute;
			}
		}

		return fs::path();
	}

	fs::path ReportParser::defaultFilename() const
	{
		return fs::path("");
	}

	std::string ReportParser::parseName(const std::string& section, bool& error) const
	{
		if (error)
			return std::string();

		return section;
	}

	std::string ReportParser::parseIsin(const std::string& section, bool& error) const
	{
		if (error)
			return std::string();

		std::string isin = section;
		error = !ValidateInput::isin(isin);

		if (error)
			return std::string();

		return isin;
	}

	uint32_t ReportParser::parseQuantity(const std::string& section, bool& error) const
	{
		uint32_t quantity = 0;

		if (error)
			return quantity;

		try
		{
			quantity = static_cast<uint32_t>(std::stoul(section));
			if (!ValidateInput::quantity(quantity))
				throw;
		}
		catch (...)
		{
			error = true;
		}

		return quantity;
	}

	Currency ReportParser::parseCurrency(const std::string& section, bool& error) const
	{
		auto currency = ccy::stoc(section);

		if (!currency.has_value() || !Forex::availableCurrency(currency.value()))
		{
			error = true;
			return Currency::EUR;
		}

		return currency.value();
	}




	// DEGIRO

	ReportParser::Output DegiroReportParser::extractSecurities(std::ifstream& stream) const
	{
		// Output
		Status status = Status::SUCCESS;
		ParserType type = ParserType::DEGIRO;
		ReportParser::Output::Securities securities;

		// Start parsing file
		std::string line;

		// Skip Header
		std::getline(stream, line);

		// Parse securities
		for (; std::getline(stream, line); )
		{
			// Skip cash lines
			if (line.substr(0, line.find(' ')) == "CASH")
				continue;

			// Parser utilities
			size_t columnStart;
			size_t columnEnd;
			auto readSection = [line, &columnStart, &columnEnd]() -> std::string const
			{
				return std::string(line.begin() + columnStart, line.begin() + columnEnd);
			};

			bool error = false;

			// Parse name
			columnStart = 0;
			columnEnd = line.find_first_of(',', columnStart + 1);
			std::string name = parseName(readSection(), error);

			// Parse isin
			columnStart = columnEnd + 1;
			columnEnd = line.find_first_of(',', columnStart + 1);
			std::string isin = parseIsin(readSection(), error);

			// Parse quantity
			columnStart = columnEnd + 1;
			columnEnd = line.find_first_of(',', columnStart + 1);
			uint32_t quantity = parseQuantity(readSection(), error);

			// Parse price: move section even if we do not parse the DEGIRO price since
			// it actually refers to the last day closing price
			columnStart = columnEnd + 2; // + 2 since there is a " character after the ,
			columnEnd = line.find_first_of('"', columnStart + 1);

			// Parse currency
			columnStart = columnEnd + 2;	// + 2 since there is a " character before the ,
			columnEnd = columnStart + 3;	// 3 letter currency
			Currency currency = parseCurrency(readSection(), error);
			
			// Check parsing status
			if (error)
			{
				securities.push_back(SecurityShell(isin, name));
				continue;
			}

			// Fetch quote (quotes are not parsed from DEGIRO report)
			auto optQuote = http::Api::fetchQuoteFor(isin, currency);
			if (!optQuote.has_value())
			{
				securities.push_back(SecurityShell(isin, name));
				continue;
			}

			Quote quote{ optQuote.value() };

			// Try to construct a security
			try 
			{
				securities.push_back(Security(isin, name, quantity, quote));
			}
			catch (...)
			{
				securities.push_back(SecurityShell(isin, name));
				continue;
			}
		}

		return { status, type, std::move(securities) };
	}

	fs::path DegiroReportParser::defaultFilename() const
	{
		return Config::properties().get<std::string>("parser.degiro.defaultFilename");
	}

}




		