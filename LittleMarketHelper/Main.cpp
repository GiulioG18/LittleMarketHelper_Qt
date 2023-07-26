
#include "observable.h"
#include "Null.h"
#include "LazyObject.h"
#include "FinProduct.h"
#include "Portfolio.h"
#include "Logger.h"

#include <iostream>

// TODO: write logs for existing code!


int main()
{
	try
	{
		ASSERT(lmh::Logger::initialize("C:/Users/giuli/OneDrive/Desktop/test.txt", LOG_LEVEL_MAX), "log init failed");

		INFO("this is an info");
		WARNING("this is a warning");
		ERROR("this is an error");
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
}