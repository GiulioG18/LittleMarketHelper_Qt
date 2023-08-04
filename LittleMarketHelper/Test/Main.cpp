
#include "Patterns/Observable.h"
#include "Patterns/LazyObject.h"
#include "Utils/Null.h"
#include "Utils/Logger.h"
#include "FinProduct.h"
#include "Portfolio.h"
#include "ReportParser.h"
#include "DegiroReportParser.h"

#include <iostream>

// TODO: write logs for existing code!


using namespace lmh;


class A
{
public:

	virtual void f() const { std::cout << "BASE CLASS f()" << std::endl; };
	virtual void g() const = 0;
	void n() { f(); }
};

class B : public A
{
public:

	virtual void f() const override { std::cout << "DERIVED (B) CLASS f()" << std::endl; };
	virtual void g() const override { f(); }
};

class C : public A
{
public:

	virtual void g() const override { f(); }
};



int main()
{
	try
	{
		lmh::Logger::initialize("C:/Users/giuli/OneDrive/Desktop/test.txt", LOG_LEVEL_MAX);
		if (!Logger::instance().initialized())
		{
			std::cout << "Logger uninitialized!" << std::endl;
		}

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
		product2->setPriceTo(1.2f);
		std::cout << portfolio->balance()->value() << std::endl;
		portfolio->clear();
		std::cout << portfolio->balance()->value() << std::endl;

		std::unique_ptr<A> b = std::make_unique<B>();
		std::unique_ptr<A> c = std::make_unique<C>();

		b->n();
		c->n();
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
}