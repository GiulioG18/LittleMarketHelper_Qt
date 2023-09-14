
#include <fstream>

#include "DegiroReportParser.h"


namespace lmh {

	DegiroReportParser::DegiroReportParser(ReportParser::Type type)
		: ReportParser(type)
	{
	}

	void DegiroReportParser::readFile(const fs::path& file, std::vector<Security>& security, bool& successful) const
	{
		REQUIRE(type_ == ReportParser::Type::DEGIRO, "parser type is not DEGIRO");

		std::ifstream stream{ file };
		std::string line;

		auto nextLine = [&stream, &line]() -> std::string&
		{
			std::getline(stream, line);
			return line;
		};

		// Check if file is readable
		successful = stream.is_open();
		if (!successful)
			return;

		// ...and not empty
		successful = stream.peek() != std::ifstream::traits_type::eof();
		if (!successful)
			return;

		// Start parsing file
		nextLine();

		// Skip Header
		nextLine();

		// Skip cash lines
		while (line.substr(0, line.find(' ')) == "CASH")
			nextLine();

		// Parse financial products
		do
		{
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

			// Isin
			sectionStart = sectionEnd + 1;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			isin = getSection();
			if (!Security::validateIsin(isin))
				continue;

			// Name
			sectionStart = 0;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			name = getSection();

			// Currency
			// TODO: ADD CURRENCY!!

			// Quantity
			sectionStart = sectionEnd + 1;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			quantity = std::stol(getSection());
			if (!Security::validateQuantity(quantity))
				continue;

			// Price
			sectionStart = sectionEnd + 2; // + 2 since there is a " character after the ,
			sectionEnd = line.find_first_of('"', sectionStart + 1);
			std::replace(line.begin() + sectionStart, line.begin() + sectionEnd, ',', '.');
			price = std::stof(getSection());
			if (!Security::validatePrice(price))
				continue;

			security.push_back(Security(isin, name, ccy, quantity, price));

			nextLine();

		} while (!line.empty());

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