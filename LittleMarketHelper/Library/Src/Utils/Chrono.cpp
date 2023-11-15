
#include "Utils/Chrono.h"


namespace lmh {

	Date::Date(const Year& year, const Month& month, const Day& day, const Hours& hours, const Minutes& minutes, const Seconds& seconds)
	{
	}

	Date::Date(const Timepoint& tPoint)
		: timepoint_(std::make_optional<Timepoint>(tPoint))
	{
	}

	std::ostream& operator<<(std::ostream& os, const Date& date)
	{
		if (date.timepoint_.has_value())
		{
			std::time_t time = std::chrono::system_clock::to_time_t(*date.timepoint_);
			os << std::put_time(std::localtime(&time), "%F %T");
		}
		else
		{
			os << "Null date" << std::endl;
		}
		
		return os;
	}

}