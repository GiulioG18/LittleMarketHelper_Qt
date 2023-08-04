CMAKE_MINIMUM_REQUIRED(VERSION 3.20.3)

PROJECT(LittleMarketHelper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check for Boost env. variable
IF(NOT DEFINED ENV{Boost})
MESSAGE(SEND_ERROR "Please set environment variable 'Boost' before building")
ENDIF()

# Source files
SET(SRC
	Src/Portfolio.cpp
	Src/FinProduct.cpp
	Src/Balance.cpp
	Src/ReportParser.cpp
	Src/DegiroReportParser.cpp
	Src/Weight.cpp
	Src/Tradeset.cpp
	Src/Utils/Errors.cpp
	Src/Utils/Logger.cpp
	Src/Patterns/Observable.cpp
)

# Header files
SET(HEADER
	Header/Balance.h
	Header/Currency.h
	Header/DegiroReportParser.h
	Header/FinProduct.h
	Header/Portfolio.h
	Header/ReportParser.h
	Header/Tradeset.h
	Header/Weight.h
	Header/Utils/Assertions.h
	Header/Utils/Environment.h
	Header/Utils/Errors.h
	Header/Utils/Logger.h
	Header/Utils/Null.h
	Header/Utils/File.h
	Header/Patterns/LazyObject.h
	Header/Patterns/Observable.h
	Header/Patterns/Singleton.h
)

ADD_LIBRARY(LittleMarketHelper STATIC
	${SRC}
	${HEADER}
)

TARGET_INCLUDE_DIRECTORIES(LittleMarketHelper PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/Header	
	$ENV{Boost}
)

# For VS, to maintain the current directory tree inside the Solution Explorer
SOURCE_GROUP(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
SOURCE_GROUP(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER})

# Set Build type to either DEVELOPMENT or DEPLOYMENT
SET(BUILD_TYPE "DEVELOPMENT" CACHE STRING "Either DEVELOPMENT (Default value) or DEPLOYMENT")
IF(BUILD_TYPE STREQUAL DEVELOPMENT)
MESSAGE(STATUS "Build type: DEVELOPMENT")
TARGET_COMPILE_DEFINITIONS(LittleMarketHelper PUBLIC _DEVELOPMENT)
ELSEIF(BUILD_TYPE STREQUAL DEPLOYMENT)
MESSAGE(STATUS "Build type: DEPLOYMENT")
TARGET_COMPILE_DEFINITIONS(LittleMarketHelper PUBLIC _DEVELOPMENT)
ELSE()
MESSAGE(SEND_ERROR "Unknown build type, select either DEVELOPMENT or DEPLOYMENT")
TARGET_COMPILE_DEFINITIONS(LittleMarketHelper PUBLIC _)
ENDIF()










