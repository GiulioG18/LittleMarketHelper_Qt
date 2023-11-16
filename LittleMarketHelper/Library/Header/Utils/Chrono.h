
// ========================================================================
//		Chrono		
// ========================================================================

#pragma once

#include <chrono>


using namespace std::literals::chrono_literals;


namespace lmh {

	// Forward declarations
	class Chrono;

	// Hard types for calendar elements
	using Year = std::chrono::year;
	using Month = std::chrono::month;
	using Day = std::chrono::day;
	using Hours = std::chrono::hours;			
	using Minutes = std::chrono::minutes; // Minits
	using Seconds = std::chrono::seconds;		


	// . allow arithmetic operations with durations
	// . utility stuff to calculate days/hours/minutes between dates
	// . deal with UTC / local time problem
	// . spaceship operator, equality operator
	
	// Wrapper around Chrono library
	// Stores a local date (technically: a pair of time_zone and time_point)
	class Date
	{
	private:

		using Clock = std::chrono::system_clock;
		using Timepoint = Clock::time_point;
		using ZonedTimepoint = std::chrono::zoned_time<Clock::duration>;
		using Timezone = std::chrono::time_zone;
		using UnixTimestamp = Seconds;

		friend std::ostream& operator<<(std::ostream& os, const Date& timepoint);
		friend class Chrono;

	public:

		Date() = default;													// Construct a date with UTC zone and epoch timepoint
		Date(const Timezone* tz, const Timepoint& timepoint);				// Constructs a date with the given zone and given timepoint
		Date(const Timepoint& timepoint);									// Constructs a date with the current zone and given timepoint
		Date(const Timezone* tz, const UnixTimestamp& timestamp);			// Constructs a date with the given zone and given UTC timestamp in seconds
		Date(const UnixTimestamp& timestamp);								// Constructs a date with the current zone and given UTC timestamp in seconds
		Date( // [ MAY THROW ]												// Constructs a date from calendar objects + duration objects
			const Year& year,
			const Month& month,
			const Day& day,
			const Hours& hours,
			const Minutes& minutes,
			const Seconds& seconds);

		const auto timepoint() const;
		const auto localTimepoint() const;
		std::string_view timezone() const;
		void prettyPrint(std::ostream& os) const;

		// Operators
		auto operator<=>(const Date& other) const;
		auto operator==(const Date& other) const;

	private:

		ZonedTimepoint zonedTime_;
	};

	
	std::ostream& operator<<(std::ostream& os, const Date& timepoint);




	// Utility class for time related functions
	class Chrono
	{
	public:

		static Seconds secondsBetween(const Date& before, const Date& after);
		static Minutes minutesBetween(const Date& before, const Date& after);
		static Hours hoursBetween(const Date& before, const Date& after);
		static uint32_t daysBetween(const Date& before, const Date& after);
		static Date::Timepoint now();
		static const Date::Timezone* here();
	};

}
