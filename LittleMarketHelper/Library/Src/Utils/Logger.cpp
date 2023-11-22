
#include <set>

#include "Utils/Logger.h"
#include "Utils/File.h"
#include "Config.h"


namespace lmh {

	// Maximum number of log files allowed at the same time
	const uint32_t MAX_LOG_FILES = static_cast<uint32_t>(5);

	Logger::~Logger()
	{
		if (!initialized_) return;

		if (stream_->is_open())
		{
			// TODO2: if user modifies the file there could be troubles
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

	Status Logger::initialize(LogLevel logLevel)
	{
		assert(Config::get().initialized());

		// Create the logger in memory
		Logger& logger = Singleton<Logger>::get();

		// Assures log is only initialized once.
		// This is to ensure that the stream is not redirected midway
		if (logger.initialized_)
			return Status::LOGGER_ALREADY_INITIALIZED;

		// Initialize folder directory
		logger.folder_ = Config::properties().get<std::string>("logger.directory");

		if (!fs::is_directory(logger.folder_))
			return Status::INVALID_DIRECTORY;
		if (!File::writable(logger.folder_))		
			return Status::NO_WRITE_PERMISSION;
		
		// Initialize time
		logger.date_ = Chrono::now();

		// Initialize file
		logger.file_ = logger.folder_;
		logger.file_.append(std::string("Log_") + std::to_string(logger.date_.timestamp()) + std::string(".txt"));

		// Initialize log level
		logger.logLevel_ = logLevel;
		if (logger.logLevel_ > LOG_LEVEL_MAX)		
			logger.logLevel_ = LOG_LEVEL_DEFAULT;
		
		// Initialize stream
		logger.stream_ = std::make_unique<std::fstream>(logger.file_, std::ios_base::out | std::ios_base::trunc);
		if (!logger.stream_->is_open())			
			return Status::FILE_NOT_OPEN;	

		// Initialize maximum log files allowed
		logger.maximumLogsAllowed_ = Config::properties().get<uint32_t>("logger.maxLogFiles");

		// Finalize
		logger.writeLogHeader();
		logger.initialized_ = true;

		return Status::SUCCESS;
	}

	void Logger::writeLogHeader() const
	{
		*(stream_) << "Little Market Helper LOG taken on " << date_ << std::endl;
		*(stream_) << "=======================================================" << std::endl << std::endl;
	}

	void Logger::ControlLogPopulation() const
	{
		// Log files in the set will be ordered based on the unix timestamp
		std::set<std::string> logs;

		for (auto const& entry : std::filesystem::directory_iterator(folder_))
		{
			std::string fileName = entry.path().filename().string();

			// Must be a log file of the form: Log_123.txt
			assert(fileName.substr(0, fileName.find('_')) == "Log");

			logs.emplace(std::move(fileName));
		}

		while (logs.size() > maximumLogsAllowed_)
		{
			try
			{
				// Remove oldest (smallest time) log from logs folder
				auto oldest = std::begin(logs);	
				fs::remove(folder_ / fs::path(*oldest));

				// Erase from set
				logs.erase(oldest);
			}
			catch (std::exception& e)
			{
				FAIL("could not remove log");
			}
		}
	}

}