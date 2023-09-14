
#include "Patterns/Observable.h"
#include "Patterns/LazyObject.h"
#include "Utils/Logger.h"
#include "Security.h"
#include "Portfolio.h"
#include "ReportParser.h"
#include "DegiroReportParser.h"
#include "Utils/File.h"
#include "Weight.h"
#include "Calibrator.h"

#include <iostream>


using namespace lmh;


// TOOD:
// 
// . important solution for ur problems: add error code!
// 
// . write the damn comments
// 
// . BUG! adding a trade to the portoflio should also check the Excluded tradeset!
//
// . Safely (!!!) query from WEB servers
// 
// . create editTrade function inside Portfolio
// 
// . create user settings object (currency, ...)
// 
// . create repository object (exchange rates map, ...)
// 
// . add currency to portfolio and make it observed by it somehow...
// 
// . Make Currency inside Portfolio relevant!
//		- this will be selected by the user and will be relevant for the prices
// 
// . Create exchange rates for portfolio's Currency
// 
// . add credentials for users
// 
// . User takes snapshots of portfolio that gets saved (somewhere... maybe free Database?)
// 
// . add stats for user investment history (IRR, holding history, holding's IRR, ...)
// 
// . would be a performance benefit to use "freeze" inside observers
// 
// . add concept of session
// 
// 
// 
// ...... QT........ 
// 
// 
// . login context
//		- store username after asking...
//	
// . concept of session (might need to create it also in lmh)
// 
//




int main()
{
	try
	{
		gogo();

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

		std::shared_ptr<Security> security = MakeSecurity("gay prod")
			.withPrice(12.2f)
			.withQuantity(12);
		std::shared_ptr<Security> security2 = MakeSecurity("gay prod2")
			.withPrice(0.2f)
			.withQuantity(21);

		std::unique_ptr<Portfolio> portfolio = std::make_unique<Portfolio>();
		portfolio->add(security2);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->add(security);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->remove("gay prod");
		std::cout << portfolio->balance()->value() << std::endl;
		security2->setPriceTo(1.2f);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->remove("gay prod2");
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->clear();
		std::cout << portfolio->balance()->value() << std::endl;
		security2->setPriceTo(1.55f);
		portfolio->remove("gay prod2");
		// When parsing from inside Portfolio we should think 
		// of catching validate user input exception
		auto parsingResults = ReportParser::parseDefault(ReportParser::Type::DEGIRO);
		if (parsingResults.second)
		{
			for (const auto& sec : parsingResults.first)
			{
				portfolio->add(std::make_shared<Security>(sec));
			}
		}
		//portfolio->exclude("ISHARES FTSE MIB UCITS ETF EUR (...");
		//portfolio->add(security2);
		//std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		//portfolio->exclude("gay prod");
		//std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		//portfolio->exclude("gay prod2");
		//std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		//portfolio->exclude("d");
		//std::cout << portfolio->balance()->value() << std::endl;
		////portfolio->include("gay prod2");
		//std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		//std::cout << portfolio->balance()->value() << std::endl; 
		//portfolio->include("gay prod222");
		//std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		//std::cout << portfolio->balance()->value() << std::endl;
		//portfolio->exclude("ISHARES CORE S&P 500 UCITS ETF ...");
		//std::cout << "excluded trades: " << portfolio->excludedTradesCount() << std::endl;
		//std::cout << portfolio->balance()->value() << std::endl;

		Calibrator cal{ *portfolio };
		Calibrator::WeightsMap wm;
		wm.emplace("ISHARES CORE S&P 500 UCITS ETF U...", 0.65f);
		wm.emplace("AMUNDI ETF MSCI EM"					, 0.15f);
		wm.emplace("LYXOR STOXX EUROPE 600 (DR) UCIT...", 0.15f);
		wm.emplace("SPDR RUSSELL 2000 US SMALL CAP U...", 0.05f);
		cal.runOptimization(wm, 9000.f);
		auto result = cal.result();
		result.value().cash();
		result.value().data().at(2).idealQuantity();
		Calibrator cal2(cal);
		auto result2 = result;
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
		return EXIT_FAILURE;
	}
}