
#include "Utils/Logger.h"
#include "Utils/Null.h"


namespace lmh {

	Logger::Logger()
		: logLevel_(Null<LogLevel>()), initialized_(false)
	{
	}

	Logger::~Logger()
	{
		if (!initialized_) return;

		if (stream_->is_open())
		{
			// Set log file to read-only
			fs::permissions(file_, 
				fs::perms::owner_read | 
				fs::perms::group_read |
				fs::perms::others_read, 
				fs::perm_options::replace);
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

		logger.file_ = filePath;
		// TODO: should (especially) care to check if the file is writable here!
		if (!fs::is_regular_file(logger.file_))
		{
			logger.initialized_ = false;
			return logger.initialized_;
		}

		logger.stream_ = std::make_unique<std::fstream>();
		ASSERT(logger.stream_, "invalid stream");
		logger.stream_->open(logger.file_, std::ios_base::out | std::ios_base::trunc);
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