
#include <fstream>

#include "DegiroReportParser.h"
#include "Utils/Null.h"


namespace lmh {

	DegiroReportParser::DegiroReportParser(ReportParser::Type type)
		: ReportParser(type)
	{
	}

	void DegiroReportParser::readFile(const fs::path& file, std::vector<FinProduct>& products, bool& successful) const
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
			int quantity = Null<int>();
			float price = Null<float>();

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
			if (name.empty())
				continue;

			// Isin
			sectionStart = sectionEnd + 1;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			isin = getSection();
			if (!isin.empty() && isin.length() != 12)
				isin = "";

			// Quantity
			sectionStart = sectionEnd + 1;
			sectionEnd = line.find_first_of(',', sectionStart + 1);
			quantity = std::stol(getSection());
			if (quantity < 0)
				continue;

			// Price
			sectionStart = sectionEnd + 2; // + 2 since there is a " character after the ,
			sectionEnd = line.find_first_of('"', sectionStart + 1);
			std::replace(line.begin() + sectionStart, line.begin() + sectionEnd, ',', '.');
			price = std::stof(getSection());
			if (price <= 0)
				continue;

			FinProduct product = MakeFinProduct(name)
				.withIsin(isin)
				.withQuantity(quantity)
				.withPrice(price);

			products.push_back(std::move(product));

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