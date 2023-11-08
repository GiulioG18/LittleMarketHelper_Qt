
// ========================================================================
//		Chrono		
// ========================================================================

#pragma once

#include <chrono>
#include <optional>


using namespace std::literals;

namespace lmh {

	// Hard types for calendar elements
	using Year = std::chrono::year;
	using Month = std::chrono::month;
	using Day = std::chrono::day;
	using Hours = std::chrono::hours;					// Duration Type
	using Minutes = std::chrono::minutes; // Minits		// Duration Type
	using Seconds = std::chrono::seconds;				// Duration Type


	// TODO2: properly impl
	
	// Wrapper around Chrono library time_point class for optional timestamp
	class Timestamp
	{
	private:

		using Clock = std::chrono::system_clock;
		using TPoint = std::chrono::time_point<Clock>;

	public:

		friend std::ostream& operator<<(std::ostream& os, const Timestamp& tPoint);

	public:

		Timestamp() = default; // Construct a null timestamp

		Timestamp(
			const Year& year,
			const Month& month,
			const Day& day,
			const Hours& hours,
			const Minutes& minutes, 
			const Seconds&	seconds);

		Timestamp(const TPoint& tPoint);

	private:

		std::optional<TPoint> tPoint_;
	};

	std::ostream& operator<<(std::ostream& os, const Timestamp& timestamp);

}
