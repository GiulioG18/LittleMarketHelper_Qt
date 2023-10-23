
#include <fstream>

#include "DegiroReportParser.h"
#include "Http/Api.h"


namespace lmh {

	// TODO: would be safer to create tokens for each column instead of parsing character by character
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
			Currency::Type ccy;

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

			// Currency::Type
			columnStart = columnEnd + 2;	// + 2 since there is a " character before the ,
			columnEnd = columnStart + 3;	// 3 letter ccy
			std::string ccyStr = getSection();
			bool validCcy = false;
			for (const auto& it : Currency::_typeMap)
			{
				if (it.second == ccyStr)
				{
					ccy = it.first;
					validCcy = true;
					break;
				}
			}

			if (!validCcy)
			{
				output.parsedSecurities_.push_back(ReportParser::UncompleteSecurity(isin, name));
				continue;
			}

			// DEGIRO does not report neither a valid price nor a ticker for a given security
			// This means we need to first get the tickers from the isin, and then request
			// the price from the tickers until we find a good one
			
			// Request quote, if not succesful, discard this product			
			YahooFinanceApi yf;	// TODO: move as member, maybe create also a member function that returns a std::optional<Quote> here
			std::optional<Quote> q = yf.getQuoteFromIsin(isin);
			// TODO: make sure that: 
			// . q has value, otherwise --> uncomplete
			// . q currency is the same as ccy, otherwise --> uncomplete
			// . q is valid, otherwise --> uncomplete			

			// Everything looks good, construct a security
			output.parsedSecurities_.push_back(Security(isin, name, quantity, q.value()));
			output.found_++;
		}		

		return Status::SUCCESS;
	}

	fs::path DegiroReportParser::defaultFilename() const
	{
		return fs::path("Portfolio");
	}

	fs::path DegiroReportParser::defaultExtension() const
	{
		return fs::path(".csv");
	}

}




