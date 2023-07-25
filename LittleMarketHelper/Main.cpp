
#include "observable.h"
#include "Null.h"
#include "LazyObject.h"
#include "FinProduct.h"
#include "Portfolio.h"

#include <iostream>


int main()
{
	try
	{
		std::shared_ptr<lmh::Portfolio> p = std::make_shared<lmh::Portfolio>();
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
}