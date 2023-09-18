
#include <set>

#include "Utils/Logger.h"
#include "Utils/File.h"
#include "Config.h"

namespace lmh {

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

	LmhStatus Logger::initialize(const fs::path& folder, LogLevel logLevel)
	{
		// Create the logger in memory
		Logger& logger = Singleton<Logger>::instance();

		// Assures log is only initialized once.
		// This is to ensure that the stream is not redirected midway
		if (logger.initialized_)
			return LmhStatus::LOGGER_ALREADY_INITIALIZED;

		// Initialize folder directory
		if (folder.empty())
		{			
			if (!Config::instance().initialized())
				return LmhStatus::CONFIG_NOT_INITIALIZED;

			// Read the folder from the config file
			auto dir = Config::instance().get<std::string>("logger.directory");
			if (!dir.has_value())
				return LmhStatus::CONFIG_INVALID_PATH;

			logger.folder_ = dir.value();
		}
		else
		{
			logger.folder_ = folder;
		}

		if (!fs::is_directory(logger.folder_))
			return LmhStatus::INVALID_DIRECTORY;
		if (!File::writable(logger.folder_))		
			return LmhStatus::NO_WRITE_PERMISSION;
		
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
			logger.logLevel_ = LOG_LEVEL_DEFAULT;
		
		// Initialize stream
		logger.stream_ = std::make_unique<std::fstream>(logger.file_, std::ios_base::out | std::ios_base::trunc);
		ASSERT(logger.stream_, "invalid stream");
		if (!logger.stream_->is_open())			
			return LmhStatus::FILE_NOT_OPEN;	

		// Initialize max log files from config file
		auto dir = Config::instance().get<int>("logger.maxLogFiles");
		if (!dir.has_value())
			return LmhStatus::CONFIG_INVALID_PATH;
		logger.maxLogFiles_ = dir.value();

		// Finalize
		logger.writeLogHeader();
		logger.initialized_ = true;

		return LmhStatus::SUCCESS;
	}

	void Logger::writeLogHeader() const
	{
		*(stream_) << "Little Market Helper LOG taken on " << std::ctime(&time_)
			<< std::endl;
		*(stream_) << "=============================================================="
			<< std::endl << std::endl;
	}

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

		while (logTimes.size() > maxLogFiles_)
		{
			// Remove oldest (smallest time) Log
			auto oldest = std::begin(logTimes);
			std::string file = "Log_" + std::to_string(*oldest) + ".txt";
			logTimes.erase(oldest);

			try
			{
				fs::remove(file);	// Remove file from host env
			}
			catch (std::exception& e)
			{
				// TODO: dilemma, who do we tell if can't remove file?
			}
		}
	}

}