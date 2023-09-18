
#include <fstream>

#include "DegiroReportParser.h"
#include "ConfigRequest.h"


namespace lmh {

	LmhStatus DegiroReportParser::readFile(const fs::path& file, Output& output) const
	{
		REQUIRE(output.type_ == ReportParser::Type::DEGIRO, "parser type is not DEGIRO");

		// Check if file is readable
		std::ifstream stream{ file };
		bool open = stream.is_open();
		if (!open)
			return LmhStatus::FILE_NOT_OPEN;		

		// ...and not empty
		bool empty = stream.peek() == std::ifstream::traits_type::eof();
		if (empty)
			return LmhStatus::FILE_IS_EMPTY;

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
			float price = 0.0f;
			Currency ccy;

			size_t sectionStart;
			size_t sectionEnd;
			auto getSection = [line, &sectionStart, &sectionEnd]() -> std::string const
			{
				return std::string(line.begin() + sectionStart, line.begin() + sectionEnd);
			};

			// Name
			sectionStart = 0;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			name = getSection();

			// Isin
			sectionStart = sectionEnd + 1;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			isin = getSection();
			if (!Security::validateIsin(isin))
			{
				output.discarded_++;
				continue;
			}

			// Quantity
			sectionStart = sectionEnd + 1;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			quantity = std::stol(getSection());
			if (!Security::validateQuantity(quantity))
			{
				output.parsedSecurities_.push_back(ReportParser::UncompleteSecurity(isin, name));
				continue;
			}

			// Price
			// Move iterators even if we do not parse the DEGIRO price since
			// it actually refers to the last day closing price...
			sectionStart = sectionEnd + 2; // + 2 since there is a " character after the ,
			sectionEnd = line.find_first_of('"', sectionStart + 1);

			// Currency
			sectionStart = sectionEnd + 2;	// + 2 since there is a " character before the ,
			sectionEnd = sectionStart + 3;	// 3 letter ccy
			std::string ccyStr = getSection();
			bool validCcy = false;
			for (const auto& it : ccyStringMap)
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

			// Request actual price	for this currency
			//ConfigRequest tr; // TODO: read URL and NODE for DEGIRO PRICE_REQUEST in the config.json file
			//LmhStatus status = tr.request(&price);
			/*if (!Security::validatePrice(price) || status != LmhStatus::SUCCESS)
			{
				output.parsedSecurities_.push_back(ReportParser::UncompleteSecurity(isin, name));
				continue;
			}*/

			// If everything went ok, add the security
			output.parsedSecurities_.push_back(Security(isin, name, ccy, quantity, price));
			output.found_++;
		}		

		return LmhStatus::SUCCESS;
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




