
// ========================================================================
//		Logger		
// ========================================================================

#pragma once

#include <fstream>
#include <vector>
#include <filesystem>
#include <memory>
#include <chrono>
#include <ctime>

#include "Assertions.h"
#include "Singleton.h"
#include "Null.h"


using LogLevel = unsigned int;

// Log levels
#define LOG_LEVEL_DEFAULT			LOG_LEVEL_INFO
#define LOG_LEVEL_MAX				LOG_LEVEL_INFO
#define LOG_LEVEL_INFO				static_cast<LogLevel>(3)
#define LOG_LEVEL_WARNING			static_cast<LogLevel>(2)
#define LOG_LEVEL_ERROR				static_cast<LogLevel>(1)
// Set colors
#ifdef LOG_SET_NO_COLORS
#define LOG_INFO_MSG				"[ INFO    ]:	"
#define LOG_WARNING_MSG				"[ WARNING ]:	"
#define LOG_ERROR_MSG				"[ ERROR   ]:	"
#else //TODO: Define colors
#define LOG_INFO_MSG				"[ INFO    ]:	"
#define LOG_WARNING_MSG				"[ WARNING ]:	"
#define LOG_ERROR_MSG				"[ ERROR   ]:	"
#endif
// Logging macro meant to be used in code
#define INFO(x) lmh::Logger::instance().log<LOG_LEVEL_INFO>((x));
#define WARNING(x) lmh::Logger::instance().log<LOG_LEVEL_WARNING>((x));
#define ERROR(x) lmh::Logger::instance().log<LOG_LEVEL_ERROR>((x));


// TODO: move definitions outside of class

namespace lmh {

	class Logger : public Singleton<Logger>
	{
		friend class Singleton<Logger>;

	private:

		Logger()
			: logLevel_(Null<LogLevel>()), initialized_(false)
		{
		}

		virtual ~Logger()
		{
			if (stream_->is_open())
			{
				stream_->close();
			}
		}

	public:

		// Set stream and log level
		static bool initialize(const std::string& filePath, LogLevel logLevel = LOG_LEVEL_DEFAULT)
		{
			// Create the logger in memory
			Logger& logger = Singleton<Logger>::instance();
			
			// Make sure this is only called once.
			// This is to ensure that the stream is not redirected midway
			REQUIRE(!logger.initialized_, "logger was already initialized");

			logger.stream_ = std::make_unique<std::fstream>();
			logger.stream_->open(filePath, std::ios_base::out | std::ios_base::trunc);		
			logger.logLevel_ = logLevel;
			logger.initialized_ = true;

			if (logger.logLevel_ > LOG_LEVEL_MAX)
			{
				logger.logLevel_ = LOG_LEVEL_DEFAULT;
			}

			if (logger.stream_->is_open())
			{
				logger.writeLogHeader();
			}
			else
			{
				logger.initialized_ = false;
			}		
			
			return logger.initialized_;
		}

		template<LogLevel messageLevel>
		inline void log(const std::string& message) const
		{
			if (messageLevel <= logLevel_ && initialized_)
			{
				*(stream_) << writeLevelSeverity<messageLevel>() << message << std::endl;
			}
		}

	private:

		inline void writeLogHeader() const
		{
			const time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			*(stream_) << "Little Market Helper LOG taken on " << std::ctime(&time) 
				<< std::endl;
			*(stream_) << "==============================================================" 
				<< std::endl << std::endl;
		}

		template<LogLevel messageLevel>
		inline std::string writeLevelSeverity() const
		{
			switch (messageLevel)
			{
			case LOG_LEVEL_INFO:	return std::string(LOG_INFO_MSG);
			case LOG_LEVEL_WARNING: return std::string(LOG_WARNING_MSG);
			case LOG_LEVEL_ERROR:	return std::string(LOG_ERROR_MSG);

			default: ASSERT(false, "unknown message level");
			}
		}

		// Setters

		inline void setLogLevel(LogLevel logLevel)
		{
			logLevel_ = logLevel;
		}

	private:

		std::unique_ptr<std::fstream> stream_;
		LogLevel logLevel_;
		bool initialized_;		
	};

}


