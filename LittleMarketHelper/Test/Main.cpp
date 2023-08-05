
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

#include <iostream>

// TODO: write logs for existing code!


using namespace lmh;


int main()
{
	try
	{
		// When parsing from inside Portfolio we should think 
		// of catching validate user input exception
		ReportParser::parseDefault(ReportParser::Type::DEGIRO);

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
			.withQuantity(0);

		std::unique_ptr<Portfolio> portfolio = std::make_unique<Portfolio>();
		portfolio->addTrade(product2);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->addTrade(product);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->removeTrade("gay prod");
		std::cout << portfolio->balance()->value() << std::endl;
		product2->setPriceTo(1.2f);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->removeTrade("gay prod2");
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->clear();
		std::cout << portfolio->balance()->value() << std::endl;
		product2->setPriceTo(1.55f);
		portfolio->removeTrade("gay prod2");
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
}