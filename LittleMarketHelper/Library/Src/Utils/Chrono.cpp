
#include "Utils/Chrono.h"


namespace lmh {

	Timestamp::Timestamp(const Year& year, const Month& month, const Day& day, const Hours& hours, const Minutes& minutes, const Seconds& seconds)
	{
	}

	Timestamp::Timestamp(const TPoint& tPoint)
		: tPoint_(std::make_optional<TPoint>(tPoint))
	{
	}

	std::ostream& operator<<(std::ostream& os, const Timestamp& timestamp)
	{
		if (timestamp.tPoint_.has_value())
		{
			std::time_t time = std::chrono::system_clock::to_time_t(*timestamp.tPoint_);
			os << std::put_time(std::localtime(&time), "%F %T");
		}
		else
		{
			os << "Null timestamp" << std::endl;
		}
		
		return os;
	}

}