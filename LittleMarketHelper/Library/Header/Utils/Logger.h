
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
#define LOG_LEVEL_MAX				LOG_LEVEL_DEBUG
#define LOG_LEVEL_DEBUG				static_cast<LogLevel>(4)
#define LOG_LEVEL_INFO				static_cast<LogLevel>(3)
#define LOG_LEVEL_WARNING			static_cast<LogLevel>(2)
#define LOG_LEVEL_ERROR				static_cast<LogLevel>(1)

#define LOG_DEBUG_MSG				"[ DEBUG   ]:	"
#define LOG_INFO_MSG				"[ INFO    ]:	"
#define LOG_WARNING_MSG				"[ WARNING ]:	"
#define LOG_ERROR_MSG				"[ ERROR   ]:	"

// Logging macro meant to be used in code
#define LMH_DEBUG(x) lmh::Logger::instance().log<LOG_LEVEL_DEBUG>((x));
#define LMH_INFO(x) lmh::Logger::instance().log<LOG_LEVEL_INFO>((x));
#define LMH_WARNING(x) lmh::Logger::instance().log<LOG_LEVEL_WARNING>((x));
#define LMH_ERROR(x) lmh::Logger::instance().log<LOG_LEVEL_ERROR>((x));

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
		static bool initialize(const fs::path& folder, LogLevel logLevel = LOG_LEVEL_DEFAULT);

		// Const methods	
		template<LogLevel messageLevel> inline void log(const std::string& message) const;

		// Getters
		inline std::fstream* stream() const;
		inline LogLevel logLevel() const;
		inline bool initialized() const;

	private:

		// Const methods
		template<LogLevel messageLevel> inline std::string writeLevelSeverity() const;
		void writeLogHeader() const;
		// Fancy name: deletes oldest log file if too numerous
		void ControlLogPopulation() const;


	private:

		std::unique_ptr<std::fstream> stream_;
		LogLevel logLevel_;
		time_t time_;
		fs::path folder_;
		fs::path file_;
		bool initialized_;
	};


	// Inline definitions
	inline std::fstream* Logger::stream() const { return stream_.get(); };
	inline LogLevel Logger::logLevel() const { return logLevel_; };
	inline bool Logger::initialized() const { return initialized_; }

	template<LogLevel messageLevel>
	inline void Logger::log(const std::string& message) const
	{
		if (messageLevel <= logLevel_ && initialized_)
		{
			*(stream_) << writeLevelSeverity<messageLevel>() << message << std::endl;
		}
	}

	template<LogLevel messageLevel>
	inline std::string Logger::writeLevelSeverity() const
	{
		switch (messageLevel)
		{
		case LOG_LEVEL_DEBUG:	return std::string(LOG_DEBUG_MSG);
		case LOG_LEVEL_INFO:	return std::string(LOG_INFO_MSG);
		case LOG_LEVEL_WARNING: return std::string(LOG_WARNING_MSG);
		case LOG_LEVEL_ERROR:	return std::string(LOG_ERROR_MSG);

		default: ASSERT(false, "unknown message level");
		}
	}

}


