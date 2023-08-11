
#include "Patterns/Observable.h"
#include "Patterns/LazyObject.h"
#include "Utils/Null.h"
#include "Utils/Logger.h"
#include "FinProduct.h"
#include "Portfolio.h"
#include "ReportParser.h"
#include "DegiroReportParser.h"
#include "Utils/File.h"
#include "Weight.h"
#include "Calibrator.h"

#include <iostream>

// TODO: write logs for existing code!
// TODO: test functionalities...


using namespace lmh;


int main()
{
	try
	{
		lmh::Logger::initialize(fs::current_path(), LOG_LEVEL_MAX);
		if (!Logger::instance().initialized())
		{
			std::cout << "Logger uninitialized!" << std::endl;
		}

		INFO("this is an info");
		WARNING("this is a warning");
		ERROR("this is an error");

		const fs::path t = "C:/Users/giuli/OneDrive/Desktop/test.txt";
		File::writable(t);

		std::shared_ptr<FinProduct> product = MakeFinProduct("gay prod")
			.withPrice(12.2f)
			.withQuantity(12);
		std::shared_ptr<FinProduct> product2 = MakeFinProduct("gay prod2")
			.withPrice(0.2f)
			.withQuantity(21);

		std::unique_ptr<Portfolio> portfolio = std::make_unique<Portfolio>();
		portfolio->add(product2);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->add(product);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->remove("gay prod");
		std::cout << portfolio->balance()->value() << std::endl;
		product2->setPriceTo(1.2f);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->remove("gay prod2");
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->clear();
		std::cout << portfolio->balance()->value() << std::endl;
		product2->setPriceTo(1.55f);
		portfolio->remove("gay prod2");
		// When parsing from inside Portfolio we should think 
		// of catching validate user input exception
		auto parsingResults = ReportParser::parseDefault(ReportParser::Type::DEGIRO);
		if (parsingResults.second)
		{
			for (const auto& product : parsingResults.first)
			{
				portfolio->add(std::make_shared<FinProduct>(product));
			}
		}
		portfolio->exclude("ISHARES FTSE MIB UCITS ETF EUR ...");
		portfolio->add(product2);
		std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		portfolio->exclude("gay prod");
		std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		portfolio->exclude("gay prod2");
		std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		std::cout << portfolio->balance()->value() << std::endl;
		//portfolio->include("gay prod2");
		std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		std::cout << portfolio->balance()->value() << std::endl; 
		//portfolio->exclude("ISHARES CORE S&P 500 UCITS ETF ...");
		std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		std::cout << portfolio->balance()->value() << std::endl;

		Calibrator cal{ *portfolio };
		Calibrator::Input input;
		input.emplace("ISHARES CORE S&P 500 UCITS ETF ...", 0.65f);
		input.emplace("AMUNDI ETF MSCI EM", 0.15f);
		input.emplace("LYXOR STOXX EUROPE 600 (DR) UCI...", 0.15f);
		input.emplace("SPDR RUSSELL 2000 US SMALL CAP ...", 0.05f);
		cal.runOptimization(input, 9000.f);

	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
		return EXIT_FAILURE;
	}
}