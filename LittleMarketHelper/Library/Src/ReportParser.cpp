
#include <fstream>

#include "ReportParser.h"


namespace lmh {

	// Base

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
		file += parser->defaultExtension();			
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
					output.status_ = parser->readFile(absolutePath, output);
					if (output.status_ == Status::SUCCESS)
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
			output.status_ = parser->readFile(report, output);
		
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



	// DEGIRO

	Status DegiroReportParser::readFile(const fs::path& file, Output& output) const
	{
		REQUIRE(output.type_ == ReportParser::Type::DEGIRO, "parser type is not DEGIRO");

		// Check if file is readable
		std::ifstream stream{ file };
		bool open = stream.is_open();
		if (!open)
			return Status::FILE_NOT_OPEN;

		// ...and not empty
		bool empty = stream.peek() == std::ifstream::traits_type::eof();
		if (empty)
			return Status::FILE_IS_EMPTY;

		// Start parsing
		std::string line;

		// Skip Header
		std::getline(stream, line);

		// Try parse securities
		for (; std::getline(stream, line); )
		{
			// Skip cash lines
			if (line.substr(0, line.find(' ')) == "CASH")
				continue;

			std::string name = "";
			std::string isin = "";
			int quantity = 0;
			double price = 0.0;
			Currency currency;

			size_t columnStart;
			size_t columnEnd;
			auto getSection = [line, &columnStart, &columnEnd]() -> std::string const
			{
				return std::string(line.begin() + columnStart, line.begin() + columnEnd);
			};

			// Name
			columnStart = 0;
			columnEnd = line.find_first_of(',', columnStart + 1);
			name = getSection();

			// Isin
			columnStart = columnEnd + 1;
			columnEnd = line.find_first_of(',', columnStart + 1);
			isin = getSection();
			if (!Security::validateIsin(isin))
			{
				output.discarded_++;
				continue;
			}

			// Quantity
			columnStart = columnEnd + 1;
			columnEnd = line.find_first_of(',', columnStart + 1);
			quantity = std::stol(getSection());
			if (!Security::validateQuantity(quantity))
			{
				output.parsedSecurities_.push_back(ReportParser::UncompleteSecurity(isin, name));
				continue;
			}

			// Price
			// Move section even if we do not parse the DEGIRO price since
			// it actually refers to the last day closing price
			columnStart = columnEnd + 2; // + 2 since there is a " character after the ,
			columnEnd = line.find_first_of('"', columnStart + 1);

			// Currency
			columnStart = columnEnd + 2;	// + 2 since there is a " character before the ,
			columnEnd = columnStart + 3;	// 3 letter currency
			std::string currencyStr = getSection();
			auto optCurrency = ccy::stoc(currencyStr);
			if (optCurrency.has_value())
			{
				currency = optCurrency.value();
			}
			else
			{
				output.parsedSecurities_.push_back(ReportParser::UncompleteSecurity(isin, name));
				continue;
			}
			// Value
			/*
			// DEGIRO does not report neither a valid price nor a ticker for a given security
			// This means we need to first get the tickers from the isin, and then request
			// the price from the tickers until we find a good one

			// Request quote, if not succesful, discard this product
			YahooFinanceApi yf;	// TODO: move as member, maybe create also a member function that returns a std::optional<Quote> here
			std::optional<Quote> q = yf.getQuoteFromIsin(isin);
			// TODO: make sure that:
			// . q has value, otherwise --> uncomplete
			// . q currency is the same as currency, otherwise --> uncomplete
			// . q is valid, otherwise --> uncomplete

			// Everything looks good, construct a security
			output.parsedSecurities_.push_back(Security(isin, name, quantity, q.value()));
			output.found_++
			*/
		}

		return Status::SUCCESS;
	}

	fs::path DegiroReportParser::defaultFilename() const
	{
		return "Portfolio";
	}

	fs::path DegiroReportParser::defaultExtension() const
	{
		return ".csv";
	}
}