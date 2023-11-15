
// ========================================================================
//		Chrono		
// ========================================================================

#pragma once

#include <chrono>
#include <optional>


using namespace std::literals;

namespace lmh {

	class Date;

	using Duration = std::chrono::duration<double>;

	// Hard types for calendar elements
	using Year = std::chrono::year;
	using Month = std::chrono::month;
	using Day = std::chrono::day;
	using Hours = std::chrono::hours;					// Duration Type
	using Minutes = std::chrono::minutes; // Minits		// Duration Type
	using Seconds = std::chrono::seconds;				// Duration Type


	// TODO2: properly impl
	// . allow arithmetic operations with durations
	// . utility stuff to calculate days/hours/minutes between dates
	// . deal with UTC / local time problem
	// . spaceship operator, equality operator
	
	// Wrapper around Chrono library time_point class for optional date
	class Date
	{
	private:

		using Clock = std::chrono::system_clock;
		using Timepoint = std::chrono::time_point<Clock>;

	public:

		friend std::ostream& operator<<(std::ostream& os, const Date& tPoint);

	public:

		Date() = default; // Construct a null date

		Date(
			const Year& year,
			const Month& month,
			const Day& day,
			const Hours& hours,
			const Minutes& minutes, 
			const Seconds&	seconds);

		Date(const Timepoint& tPoint);

		static Date now()
		{
			Timepoint tp = Clock::now();
			return tp;
		}


	private:

		std::optional<Timepoint> timepoint_;
	};

	std::ostream& operator<<(std::ostream& os, const Date& date);

}
