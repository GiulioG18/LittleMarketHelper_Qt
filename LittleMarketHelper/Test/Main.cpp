#include "Utils/Warnings.h"

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
#include "Http/Curl.h"
#include "Config.h"
#include "Http/ConfigRequest.h"
#include "TickersGenerator.h"
#include "Cash.h"

#include <fstream>
#include <iostream>


using namespace lmh;


// TOOD:
// 
// . Do not fecth all the historical price when using yf
// 
// . write the damn comments
// 
// . create repository object (exchange rates map, ...)
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
// . create repository and initialize it!
// 
// . create some global initialize function for the app
//		- Config
//		- Curl
//		- ExchangeRateRepository
//		- Logger
// 
// . check for all class
//		- copy/move ctor, copy/move operator
//		- comments...
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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

int main()
{
	try
	{
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

		//std::optional<std::string> ila = config.getString("parser.type.degiro.fileExtension");
		//std::optional<bool> intinos = config.getBool("settings.portfolio.multiCcy");
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

		Curl& curl = Curl::get();
		curl.initialize();
		auto ic = curl.GETRequest("https://papi-pornstarsapi.p.rapidapi.com/pornstars/");
		std::cout << curl.StatusMessage() << std::endl;
		auto ec = curl.GETRequest("https://query1.finance.yahoo.com/v8/finance/chart/AMZN");
		auto oc = curl.GETRequest("https://catfact.ninja/fact");
		std::cout << curl.response() << std::endl;

		auto as = curl.POSTRequest("https://api.openfigi.com/v3/mapping", "[{\"idType\":\"ID_ISIN\",\"idValue\":\"US4592001014\"}]");
		/*if (status == Status::SUCCESS)
			std::cout << curl.response() << std::endl;*/

		/*auto stringa = ConfigRequest::GET("price", "AMZN", &status);
		auto stringetto = ConfigRequest::POST("ticker", "IE00B53L4X51", &status);*/

		/*Figi figi;
		if (status == Status::SUCCESS)
			status = figi.parse(stringetto);*/

		const fs::path t = "C:/Users/giuli/OneDrive/Desktop/test.txt";
		File::writable(t);

	/*	auto stringettos = ConfigRequest::GET("exchangeRate", "AUD", &status);
		if (status == Status::SUCCESS)
		{
			std::cout << stringettos << std::endl;
		}*/

		// When parsing from inside Portfolio we should think 
		// of catching validate user input exception
		auto output = ReportParser::parseDefault(ReportParser::Type::DEGIRO);
		auto output1 = ReportParser::parse(ReportParser::Type::DEGIRO, fs::path("C:/Users/giuli/Downloads/Portfolio.csv"));

		/*ReportParser::Output out;
		if (parsingResults.second)
		{
			for (const auto& sec : parsingResults.first)
			{
				portfolio->add(std::make_shared<Security>(sec));
			}
		}*/

		std::shared_ptr<Security> a1 = std::make_shared<Security>(std::string("LU0908500753"), std::string(), 8, Quote(Price(Currency::Type::EUR, 203.85), 0.0));
		std::shared_ptr<Security> b2 = std::make_shared<Security>(std::string("IE00BJ38QD84"), std::string(), 12, Quote(Price(Currency::Type::AUD, 47.43),  0.0));
		std::shared_ptr<Security> c3 = std::make_shared<Security>(std::string("IE00B5BMR087"), std::string(), 26, Quote(Price(Currency::Type::EUR,424.20), 0.0));
		Security d4 = Security(std::string("LU1681045370"), std::string(), 420, Quote(Price(Currency::Type::SEK, 4.37), 0.0));

		std::unique_ptr<Portfolio> portfolio = std::make_unique<Portfolio>(Currency::Type::EUR);
		portfolio->addSecurity(*a1);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		portfolio->addSecurity(*b2);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		portfolio->addSecurity(*c3);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		portfolio->addSecurity(d4);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		portfolio->removeSecurity("123451234512");
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		portfolio->removeSecurity("IE00BJ38QD84");
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		//portfolio->edit<EditTrade::Type::CURRENCY>("123451234511", Currency::Type::EUR);


		auto cash0 = std::make_shared<Cash>(Currency::Type::EUR, 999.99);
		auto cash1 = std::make_shared<Cash>(Currency::Type::EUR, 0.0);
		auto cash2 = std::make_shared<Cash>(Currency::Type::CHF, 140000.3);
		auto cashWrong = std::make_shared<Cash>(Currency::Type::CHF, 124);

		portfolio->addCash(*cash0);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		std::cout << portfolio->value().value() << std::endl;
		portfolio->addCash(*cash1);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		std::cout << portfolio->value().value() << std::endl;
		portfolio->addCash(*cash2);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		std::cout << portfolio->value().value() << std::endl;
		portfolio->addCash(*cashWrong);
		std::cout << portfolio->openPosition()->price().value() << std::endl;
		std::cout << portfolio->value().value() << std::endl;

		Calibrator cal(portfolio.get());
		Calibrator::WeightsMap wm;
		wm.emplace(a1->isin(), 0.15f);
		wm.emplace(b2->isin(), 0.05f);
		wm.emplace(c3->isin(), 0.65f);
		wm.emplace(d4.isin(), 0.15f);
		cal.runOptimization(wm, Currency::Type::EUR, 20038.12);
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