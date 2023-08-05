
#include "Utils/Logger.h"
#include "Utils/Null.h"
#include "Utils/File.h"


namespace lmh {

	Logger::Logger()
		: 
		logLevel_(Null<LogLevel>()),
		time_(Null<time_t>()),
		initialized_(false)
	{
	}

	Logger::~Logger()
	{
		if (!initialized_) return;

		// TODO: clean oldest Log_* if there are more than 5

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

	bool Logger::initialize(const fs::path& folder, LogLevel logLevel)
	{
		// Create the logger in memory
		Logger& logger = Singleton<Logger>::instance();

		// Assures log is only initialized once.
		// This is to ensure that the stream is not redirected midway
		REQUIRE(!logger.initialized_, "logger was already initialized");

		// Initialize folder
		logger.folder_ = folder;
		if (!File::writable(logger.folder_))
		{
			logger.initialized_ = false;
			return logger.initialized_;
		}
		
		// Initialize time
		logger.time_ = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		// Initialize file
		logger.file_ = "Log_" + std::to_string(logger.time_) + ".txt";

		// Initialize log level
		logger.logLevel_ = logLevel;
		if (logger.logLevel_ > LOG_LEVEL_MAX)
		{
			logger.logLevel_ = LOG_LEVEL_DEFAULT;
		}

		// Initialize stream
		logger.stream_ = std::make_unique<std::fstream>(logger.file_, std::ios_base::out | std::ios_base::trunc);
		ASSERT(logger.stream_, "invalid stream");
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

	void Logger::writeLogHeader() const
	{
		*(stream_) << "Little Market Helper LOG taken on " << std::ctime(&time_)
			<< std::endl;
		*(stream_) << "=============================================================="
			<< std::endl << std::endl;
	}

}