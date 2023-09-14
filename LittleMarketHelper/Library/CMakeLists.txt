Cmake_Minimum_Required(VERSION 3.20.3)

Project(LittleMarketHelper VERSION 1.0)

Set(CMAKE_CXX_STANDARD 23)
Set(CMAKE_CXX_STANDARD_REQUIRED ON)
Set(CMAKE_CXX_EXTENSIONS OFF)

# Check for user defined variables
# Boost
If(NOT DEFINED ENV{Boost})
Message(SEND_ERROR "Please set environment variable 'Boost' before building")
Endif()
# Curl debug
If(NOT DEFINED ENV{Curl_debug})
Message(SEND_ERROR 
	"Please set environment variable 'Curl_debug' pointing to CURL static debug build before building")
Endif()
# Curl release
If(NOT DEFINED ENV{Curl_release})
Message(SEND_ERROR	
	"Please set environment variable 'Curl_release' pointing to CURL static release build before building")
Endif()


# Source files
Set(SRC
	Src/Portfolio.cpp
	Src/Security.cpp
	Src/Balance.cpp
	Src/ReportParser.cpp
	Src/DegiroReportParser.cpp
	Src/Weight.cpp
	Src/Tradeset.cpp
	Src/Calibrator.cpp
	Src/CalibrationResult.cpp
	Src/Optimizers.cpp
	Src/ExchangeRate.cpp
	Src/Utils/Errors.cpp
	Src/Utils/Logger.cpp
	Src/Patterns/Observable.cpp
)

# Header files
Set(HEADER
	Header/Balance.h
	Header/Currency.h
	Header/DegiroReportParser.h
	Header/Security.h
	Header/Portfolio.h
	Header/ReportParser.h
	Header/Tradeset.h
	Header/Weight.h
	Header/Calibrator.h
	Header/CalibrationResult.h
	Header/Optimizers.h
	Header/ExchangeRate.h
	Header/PortfolioUtilities.h
	Header/Utils/Assertions.h
	Header/Utils/Environment.h
	Header/Utils/Errors.h
	Header/Utils/Logger.h
	Header/Utils/File.h
	Header/Patterns/LazyObject.h
	Header/Patterns/Observable.h
	Header/Patterns/Singleton.h
)

Add_Library(LittleMarketHelper STATIC
	${SRC}
	${HEADER}
)

Target_Compile_Definitions(LittleMarketHelper PRIVATE CURL_STATICLIB)

Target_Include_Directories(LittleMarketHelper PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/Header	
	$ENV{Boost}
	PRIVATE $<$<CONFIG:Debug>:$ENV{Curl_debug}/include>
	PRIVATE $<$<CONFIG:Release>:$ENV{Curl_release}/include>
)

Target_Link_Libraries(LittleMarketHelper 
	debug		$ENV{Curl_debug}/lib/libcurl_a_debug.lib 
	optimized	$ENV{Curl_release}/lib/libcurl_a.lib 
)

# For VS, to maintain the current directory tree inside the Solution Explorer
Source_Group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
Source_Group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER})

# Set Build type to either DEVELOPMENT or DEPLOYMENT
Set(BUILD_TYPE "DEVELOPMENT" CACHE STRING "Either DEVELOPMENT (Default value) or DEPLOYMENT")
If(BUILD_TYPE STREQUAL DEVELOPMENT)
Message(STATUS "Build type: DEVELOPMENT")
Target_Compile_Definitions(LittleMarketHelper PUBLIC _DEVELOPMENT)
Elseif(BUILD_TYPE STREQUAL DEPLOYMENT)
Message(STATUS "Build type: DEPLOYMENT")
Target_Compile_Definitions(LittleMarketHelper PUBLIC _DEVELOPMENT)
Else()
Message(SEND_ERROR "Unknown build type, select either DEVELOPMENT or DEPLOYMENT")
Target_Compile_Definitions(LittleMarketHelper PUBLIC _)
Endif()










