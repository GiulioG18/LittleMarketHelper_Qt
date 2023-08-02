
#include "observable.h"
#include "Null.h"
#include "LazyObject.h"
#include "FinProduct.h"
#include "Portfolio.h"
#include "Logger.h"
#include "ReportParser.h"
#include "DegiroReportParser.h"

#include <iostream>

// TODO: write logs for existing code!

class A
{
public:

};

using namespace lmh;

int main()
{
	try
	{
		ASSERT(lmh::Logger::initialize("C:/Users/giuli/OneDrive/Desktop/test.txt", LOG_LEVEL_MAX), "log init failed");

		INFO("this is an info");
		WARNING("this is a warning");
		ERROR("this is an error");

		const fs::path t = "Path/Example/t.csv";

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
		product->setQuantityTo(13);
		std::cout << portfolio->balance()->value() << std::endl;
		product2->setQuantityTo(100);
		std::cout << portfolio->balance()->value() << std::endl;
		product2->setPriceTo(1.2f);
		std::cout << portfolio->balance()->value() << std::endl;
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
}