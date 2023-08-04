
// ========================================================================
//		Logger		
// ========================================================================

#pragma once

#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>

#include "Utils/Environment.h"
#include "Utils/Assertions.h"
#include "Patterns/Singleton.h"


// Log levels
#define LOG_LEVEL_DEFAULT			LOG_LEVEL_INFO
#define LOG_LEVEL_MAX				LOG_LEVEL_INFO
#define LOG_LEVEL_INFO				static_cast<LogLevel>(3)
#define LOG_LEVEL_WARNING			static_cast<LogLevel>(2)
#define LOG_LEVEL_ERROR				static_cast<LogLevel>(1)

// (TODO:) Set colors
#ifdef LOG_SET_NO_COLORS
#define LOG_INFO_MSG				"[ INFO    ]:	"
#define LOG_WARNING_MSG				"[ WARNING ]:	"
#define LOG_ERROR_MSG				"[ ERROR   ]:	"
#else 
#define LOG_INFO_MSG				"[ INFO    ]:	"
#define LOG_WARNING_MSG				"[ WARNING ]:	"
#define LOG_ERROR_MSG				"[ ERROR   ]:	"
#endif

// Logging macro meant to be used in code
#define INFO(x) lmh::Logger::instance().log<LOG_LEVEL_INFO>((x));
#define WARNING(x) lmh::Logger::instance().log<LOG_LEVEL_WARNING>((x));
#define ERROR(x) lmh::Logger::instance().log<LOG_LEVEL_ERROR>((x));

using LogLevel = unsigned int;
namespace fs = std::filesystem;


namespace lmh {

	class Logger : public Singleton<Logger>
	{
		friend class Singleton<Logger>;

	private:

		Logger();
		virtual ~Logger();

	public:

		// Non-const methods
		// Set stream, path and log level
		static bool initialize(const fs::path& filePath, LogLevel logLevel = LOG_LEVEL_DEFAULT);

		// Const methods
		inline bool initialized() const;		
		template<LogLevel messageLevel> inline void log(const std::string& message) const;

	private:

		// Non-const methods
		inline void setLogLevel(LogLevel logLevel);

		// Const methods
		template<LogLevel messageLevel> inline std::string writeLevelSeverity() const;
		inline void writeLogHeader() const;

	private:

		std::unique_ptr<std::fstream> stream_;
		fs::path file_;
		LogLevel logLevel_;
		bool initialized_;		
	};


	// Inline definitions

	inline bool Logger::initialized() const { return initialized_; }

	template<LogLevel messageLevel>
	inline void Logger::log(const std::string& message) const
	{
		if (messageLevel <= logLevel_ && initialized_)
		{
			*(stream_) << writeLevelSeverity<messageLevel>() << message << std::endl;
		}
	}

	inline void Logger::writeLogHeader() const
	{
		const time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		*(stream_) << "Little Market Helper LOG taken on " << std::ctime(&time)
			<< std::endl;
		*(stream_) << "=============================================================="
			<< std::endl << std::endl;
	}

	template<LogLevel messageLevel>
	inline std::string Logger::writeLevelSeverity() const
	{
		switch (messageLevel)
		{
		case LOG_LEVEL_INFO:	return std::string(LOG_INFO_MSG);
		case LOG_LEVEL_WARNING: return std::string(LOG_WARNING_MSG);
		case LOG_LEVEL_ERROR:	return std::string(LOG_ERROR_MSG);

		default: ASSERT(false, "unknown message level");
		}
	}

	inline void Logger::setLogLevel(LogLevel logLevel)
	{
		logLevel_ = logLevel;
	}

}


