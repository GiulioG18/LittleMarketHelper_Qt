
#include "Logger.h"
#include "Null.h"


namespace lmh {

	Logger::Logger()
		: logLevel_(Null<LogLevel>()), initialized_(false)
	{
	}

	Logger::~Logger()
	{
		if (stream_->is_open())
		{
			stream_->close();
		}
	}

	// Set stream and log level
	bool Logger::initialize(const fs::path& filePath, LogLevel logLevel)
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

}