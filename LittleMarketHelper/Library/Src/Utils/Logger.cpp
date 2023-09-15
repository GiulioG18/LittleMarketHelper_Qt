
#include <set>

#include "Utils/Logger.h"
#include "Utils/File.h"

#define MAX_LOG_FILES 5
#include <iostream>

namespace lmh {

	Logger::Logger()
		: 
		logLevel_(LOG_LEVEL_DEFAULT),
		time_(static_cast<time_t>(0)),
		initialized_(false)
	{
	}

	Logger::~Logger()
	{
		if (!initialized_) return;

		if (stream_->is_open())
		{
			// TODO: if user modifies the file there could be troubles
			//		 at the moment i'm not sure how the groups/permissions work
			fs::permissions(file_, 
				fs::perms::owner_all	| 
				fs::perms::group_read	|
				fs::perms::others_read, 
				fs::perm_options::replace);

			stream_->close();
		}

		// Clean oldest Log if there are more than MAX_LOG_FILES
		ControlLogPopulation();
	}

// TODO: REMOVE BOOL AND RETURN CODE!!
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
		logger.file_ = logger.folder_;
		logger.file_ += fs::path(
			std::string("//") + 
			std::string("Log_") + 
			std::to_string(logger.time_) + ".txt");

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

	// TODO: NOT WORKING; CHECK!!
	void Logger::ControlLogPopulation() const
	{
		// Log format is:
		// Log_123.txt

		std::set<int> logTimes;
		for (auto const& entry : std::filesystem::directory_iterator(folder_))
		{
			std::string fileName = entry.path().filename().string();

			// If it's not a Log file, ignore it
			if (fileName.substr(0, fileName.find('_')) != "Log")
				continue;

			auto timeExtractor =
				[](const std::string& log) -> int
			{
				int time = std::stoi(log.substr(4, log.find('.')));
				return time;
			};

			logTimes.emplace(timeExtractor(fileName));
		}

		while (logTimes.size() > MAX_LOG_FILES)
		{
			// Remove oldest (smallest time) Log
			auto oldest = std::begin(logTimes);
			std::string file = "Log_" + std::to_string(*oldest) + ".txt";
			logTimes.erase(oldest);

			try
			{
				fs::remove(file);			// Remove file from system
			}
			catch (std::exception& e)
			{
				// TODO: dilemma, who do we tell if can't remove file?
			}
		}
	}

}