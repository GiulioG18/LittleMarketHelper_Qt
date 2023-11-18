
#include "Utils/Chrono.h"
#include "Utils/Assertions.h"


namespace lmh {

	// Date

	Date::Date(const Timezone* tz, const Timepoint& timepoint)
		: zonedTime_(tz, timepoint)
	{
	}

	Date::Date(const Timepoint& timepoint)
		: zonedTime_(Chrono::here(), timepoint)
	{
	}

	Date::Date(const Timezone* tz, const UnixTimestamp& timestamp)
	{
		const Timepoint sinceEpoch{ timestamp };
		zonedTime_ = { tz, sinceEpoch };
	}

	Date::Date(const UnixTimestamp& timestamp)
	{
		const Timepoint sinceEpoch{ timestamp };
		zonedTime_ = { Chrono::here(), sinceEpoch };
	}

	Date::Date(const Year& year, const Month& month, const Day& day, const Hours& hours, const Minutes& minutes, const Seconds& seconds)
	{
		REQUIRE(year.ok(), "invalid year");
		REQUIRE(month.ok(), "invalid month");
		REQUIRE(day.ok(), "invalid day");

		Timepoint t = std::chrono::sys_days(year / month / day) + hours + minutes + seconds;
		zonedTime_ = ZonedTimepoint(Chrono::here(), t);
	}

	const auto& Date::systemTime() const
	{
		return zonedTime_.get_sys_time();
	}

	const auto& Date::localTime() const
	{
		return zonedTime_.get_local_time();
	}	

	std::string_view Date::timezone() const
	{
		return zonedTime_.get_time_zone()->name();
	}

	int64_t Date::timestamp() const
	{
		return zonedTime_.get_sys_time().time_since_epoch().count();
	}

	bool Date::operator==(const Date& other) const
	{
		return this->zonedTime_.get_sys_time() == other.zonedTime_.get_sys_time();
	}

	void Date::prettyPrint(std::ostream& os) const
	{
		os << "==========================================================" << "\n";
		os << "Date UTC time point:		" << systemTime() << "\n";
		os << "Date local time point:		" << localTime() << "\n";
		os << "Date timezone:			" << timezone() << "\n";
		os << "==========================================================" << "\n";
	}

	std::ostream& operator<<(std::ostream& os, const Date& date)
	{
		os << date.localTime() << " " << date.timezone();
		return os;
	}



	// Chrono

	Seconds Chrono::secondsBetween(const Date& before, const Date& after)
	{
		return std::chrono::ceil<Seconds>(after.zonedTime_.get_sys_time() - before.zonedTime_.get_sys_time());
	}

	Minutes Chrono::minutesBetween(const Date& before, const Date& after)
	{
		return std::chrono::ceil<Minutes>(after.zonedTime_.get_sys_time() - before.zonedTime_.get_sys_time());
	}

	Hours Chrono::hoursBetween(const Date& before, const Date& after)
	{
		return std::chrono::ceil<Hours>(after.zonedTime_.get_sys_time() - before.zonedTime_.get_sys_time());
	}

	uint32_t Chrono::daysBetween(const Date& before, const Date& after)
	{
		// NB: floored integer division
		return std::chrono::ceil<Hours>(after.zonedTime_.get_sys_time() - before.zonedTime_.get_sys_time()).count() / 24;
	}

	Date::Timepoint Chrono::now()
	{
		return Date::Clock::now();
	}

	const Date::Timezone* Chrono::here()
	{
		return std::chrono::current_zone();
	}

}