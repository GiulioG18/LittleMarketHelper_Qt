
#include "Utils/Warnings.h"

#define BOOST_TEST_MODULE My very bad test
#include "boost/test/included/unit_test.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <fstream>
#include <iostream>

#include "Lmh.h"
#include "Patterns/Observable.h"
#include "Patterns/LazyObject.h"
#include "Utils/Logger.h"
#include "Security.h"
#include "Portfolio.h"
#include "ReportParser.h"
#include "Utils/File.h"
#include "Weight.h"
#include "Calibrator.h"
#include "Http/Curl.h"
#include "Config.h"
#include "TickersGenerator.h"
#include "Cash.h"
#include "ExchangeRate.h"



using namespace lmh;
namespace pt = boost::property_tree;


// TODO:
// 
// . write the damn comments (see doxygen todo)
// 
// . check rule of 5
// 
// . User takes snapshots of portfolio that gets saved (somewhere... maybe free Database?)
// 
// . add stats for user investment history (IRR, holding history, holding's IRR, ...)
// 
// . would be a performance benefit to use "freeze" inside observers
// 
// . add concept of session
// 
// . WRITE THE DAMN BOOST TEST SUITE THIS IS UNSAFE AF!
// 
// . check for all class
//		- copy/move ctor, copy/move operator
//		- comments...
// 
// . Add some sort of userdata to store user specific data such as:
//		- full ticker (very important! it would avoid the figiAPI request since we expect that most products are recurrent 
//		- Portfolio history
// 
// . Create doc with doxygen
// 
// . review that  all calls to func that may throw are wrapped into try-catch blocks
// 
// . create assert() macro with a message
// 
// . review all functions that may throw:
//		- add MAY THROW to definition
//		- check implementation to see if it s possible to assert instead
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

BOOST_AUTO_TEST_CASE(first_test) 
{
	try
	{
		start("D:\\Coding\\01. Visual Studio Projects\\LittleMarketHelper\\LittleMarketHelper\\config.json");

		Price price1{Currency::GBP, 100}, price2{Currency::EUR, 310};
		std::cout << std::boolalpha << (price1 == price2) << std::endl;
		std::cout << std::boolalpha << (price1 < price2) << std::endl;

		// TEST CHRONO STUFF:
		// .. ctors
		// .. utility functions
		// .. date functions (operators)
		// .. review all code that is supposed to use a date

		Date d;
		d.prettyPrint(std::cout);
		std::cout << d << std::endl;

		int64_t sinceEpoch = 1700253913LL;
		Date uts = Date(Date::UnixTimestamp(std::max(sinceEpoch, 0LL)));
		std::cout << uts << std::endl;
		uts.prettyPrint(std::cout);
		std::cout << "ts: " <<uts.timestamp() << std::endl;
		std::cout << "se" << sinceEpoch << std::endl;

		std::cout << std::boolalpha << (uts < d) << std::endl;

		Date dateTzTp{ std::chrono::locate_zone("Australia/Sydney"), Chrono::now() };
		std::cout << dateTzTp << std::endl;
		dateTzTp.prettyPrint(std::cout);
		std::cout << "ts: " << dateTzTp.timestamp() << std::endl;
		std::cout << "se: " << Chrono::now().time_since_epoch().count() << std::endl;

		pt::ptree configTree;
		pt::read_json({ "D:\\Coding\\01. Visual Studio Projects\\LittleMarketHelper\\LittleMarketHelper\\config.json" }, configTree);
		auto mc = configTree.get_optional<std::string>("logger.directory");
		if (mc)
			std::cout << (mc.value()) << std::endl;

		Status status = Status::SUCCESS;

		// Init config
		Config& config = Config::get();
		fs::path configFile{ "D:\\Coding\\01. Visual Studio Projects\\LittleMarketHelper\\LittleMarketHelper\\config.json" };
		status = config.initialize(configFile.string());
		if (status != Status::SUCCESS)
		{
			std::cout << "Config uninitialized!" << std::endl;
		}
		auto ilassss = config.properties().get<std::string>("parser.type.degiro.name");
		auto ilass = config.properties().get<std::string>("parser.type.degiro.fileExtension");
		
		// Init logger
		status = Logger::initialize();
		if (status != Status::SUCCESS)
		{
			std::cout << "Logger uninitialized!" << std::endl;
		}
		
		LMH_INFO("this is an info");
		LMH_WARNING("this is a warning");
		LMH_ERROR("this is an error");

		http::Curl& curl = http::Curl::get();
		curl.initialize();

		http::Curl::Request request1{ http::Curl::Method::GET, "https://papi-pornstarsapi.p.rapidapi.com/pornstars/", "", false };
		http::Curl::Response response1 = curl.httpRequest(request1);
		response1.prettyPrint(std::cout);

		http::Curl::Request request2{ http::Curl::Method::GET, "https://query1.finance.yahoo.com/v8/finance/chart/AMZN", ""};
		http::Curl::Response response2 = curl.httpRequest(request2);
		http::Curl::Response response2Clone = curl.httpRequest(request2);
		response2.prettyPrint(std::cout);
		std::cout << response2.data() << std::endl;
		response2Clone.prettyPrint(std::cout);

		http::Curl::Request request3{ http::Curl::Method::GET, "https://catfact.ninja/fact", "", true };
		http::Curl::Response response3 = curl.httpRequest(request3);
		http::Curl::Response response3Clone = curl.httpRequest(request3);
		response3.prettyPrint(std::cout);
		response3Clone.prettyPrint(std::cout);

		http::Curl::Request request4{ http::Curl::Method::POST, "https://api.openfigi.com/v3/mapping", "[{\"idType\":\"ID_ISIN\",\"idValue\":\"LU1681045370\"}]", true };
		http::Curl::Response response4 = curl.httpRequest(request4);
		std::cout << response4.data() << std::endl;

		const fs::path t = "C:/Users/giuli/OneDrive/Desktop/";
		std::cout << "Writable? " << File::writable(t) << std::endl;

		auto output = ReportParser::parse(ParserType::DEGIRO);
		auto output1 = ReportParser::parse(ParserType::DEGIRO, fs::path("C:/Users/giuli/Downloads/Portfolio.csv"));
		//auto output2 = ReportParser::parse(ParserType::UNDEFINED); // <--- error!


		std::shared_ptr<Security> a1 = std::make_shared<Security>(std::string("LU0908500753"), std::string(), 8, Quote(Price(Currency::EUR, 203.85)));
		std::shared_ptr<Security> b2 = std::make_shared<Security>(std::string("IE00BJ38QD84"), std::string(), 12, Quote(Price(Currency::AUD, 47.43)));
		std::shared_ptr<Security> c3 = std::make_shared<Security>(std::string("IE00B5BMR087"), std::string(), 26, Quote(Price(Currency::EUR,424.20), Chrono::now()));
		try
		{
			std::shared_ptr<Security> negativeValue = std::make_shared<Security>(std::string("IE00B5BMR087"), std::string(), 26, Quote(Price(Currency::EUR, -424.20), Chrono::now()));
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		try
		{
			std::shared_ptr<Security> d4 = std::make_shared<Security>(std::string("IE00B5BMR087"), std::string(), 26, Quote(Price(Currency::BZR, 424.20), Chrono::now()));
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		Security d4 = Security(std::string("LU1681045370"), std::string(), 420, Quote(Price(Currency::SEK, 4.37), std::chrono::system_clock::now() - 24h));

		std::unique_ptr<Portfolio> portfolio = std::make_unique<Portfolio>(Currency::EUR);
		portfolio->addSecurity(*a1);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		portfolio->addSecurity(*b2);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		portfolio->addSecurity(*c3);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		portfolio->addSecurity(d4);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		portfolio->removeSecurity("123451234512");
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		portfolio->removeSecurity("IE00BJ38QD84");
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		//portfolio->edit<Edit::Type::NAME>("IE00BJ38QD84", Currency::EUR); // Compilation error
		portfolio->edit<Edit::Type::NAME>("LU1681045370", std::string(""));
		portfolio->edit<Edit::Type::QUANTITY>("LU1681045370", -1);
		portfolio->edit<Edit::Type::QUANTITY>("LU1681045370", static_cast<uint32_t>(1));
		portfolio->edit<Edit::Type::PRICE>("LU1681045370", -1.124);


		auto cash0 = std::make_shared<Cash>(Currency::EUR, 999.99);
		auto cash1 = std::make_shared<Cash>(Currency::EUR, 0.0);
		auto cash2 = std::make_shared<Cash>(Currency::CHF, 140000.3);
		auto cashWrong = std::make_shared<Cash>(Currency::CHF, 124);

		portfolio->addCash(*cash0);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		std::cout << portfolio->value().amount() << std::endl;
		portfolio->addCash(*cash1);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		std::cout << portfolio->value().amount() << std::endl;
		portfolio->addCash(*cash2);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		std::cout << portfolio->value().amount() << std::endl;
		portfolio->addCash(*cashWrong);
		std::cout << portfolio->openPosition()->price().amount() << std::endl;
		std::cout << portfolio->value().amount() << std::endl;

		Calibrator cal(portfolio.get());
		Calibrator::WeightsMap wm;
		wm.emplace(a1->isin(), 0.15f);
		wm.emplace(b2->isin(), 0.05f);
		wm.emplace(c3->isin(), 0.65f);
		wm.emplace(d4.isin(), 0.15f);
		cal.runOptimization(wm, Currency::EUR, 20038.12);
		auto result = cal.result();
		if (result.has_value())
		{
			result.value().cash();
			result.value().data().at(2).idealQuantity();
			Calibrator cal2(cal);
			auto result2 = result;
		}
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
		std::terminate();
	}
}