
#include "Utils/Logger.h"
#include "Utils/Null.h"
#include "Utils/File.h"


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

		// Assures log is only initialized once.
		// This is to ensure that the stream is not redirected midway
		REQUIRE(!logger.initialized_, "logger was already initialized");

		// TODO: initialize file name with a name.
		//		 the parameter of this function should be the folder instead
		
		// Initialize file
		logger.file_ = filePath;
		if (!File::writable(logger.file_)) // TODO: have to first create file
		{
			logger.initialized_ = false;
			return logger.initialized_;
		}

		logger.stream_ = std::make_unique<std::fstream>();
		ASSERT(logger.stream_, "invalid stream");

		// Initialize stream and log level
		logger.stream_->open(logger.file_, std::ios_base::out | std::ios_base::trunc);
		logger.logLevel_ = logLevel;
		if (logger.logLevel_ > LOG_LEVEL_MAX)
		{
			logger.logLevel_ = LOG_LEVEL_DEFAULT;
		}

		if (logger.stream_->is_open())
		{
			logger.writeLogHeader();
			logger.initialized_ = true;
		}
		else
		{
			logger.initialized_ = false;
		}

		return logger.initialized_;
	}

}