# pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

using std::string;


    /*************\
    *  TimePoint  *
    \*************/

// a structure that holds the year, month, day, hour, and minute of time;
struct TimePoint
{
    enum Months
    {
        January = 1, February = 2, March = 3, April = 4,
        May = 5, June = 6, July = 7, August = 8,
        September = 9, October = 10, November = 11, December = 12
    };

    // purpose: finds the next month in the year
    // requires: nothing, it is a unary operator
    // returns: the next month
    friend Months& operator++(Months&);

    // purpose: finds the previous month in the year
    // requires: nothing, it is a unary operator
    // returns: the previous month
    friend Months& operator--(Months&);

    // purpose: finds the month that is offset months away
    // requires: an integer
    // returns: the month
    friend Months& operator+=(Months&, int);

    // purpose: finds the month that is offset months in the past
    // requires: an integer
    // returns: the month
    friend Months& operator-=(Months&, int);

    // purpose: finds the difference in months
    // requires: two months
    // returns: an integer
    friend int operator-(Months, Months);

    // any integer
    int year;
    // any month
    Months month;
    // day is between 1 and however many days in the month
    int day;
    // hour ranges from 0 to 23
    int hour;
    // minute ranges from 0 to 59
    int minute;

    // purpose: sets the time point to right now
    // requires: nothing
    // returns: nothing
    TimePoint();

    // purpose: sets the time point to the given parameters
    // requires: the year and the month
    // default parameters: day is the first,
    //                     hour and minutes are set to noon
    // returns: nothing
    TimePoint(int, Months, int, int, int);

    // purpose: copies the time from another time point
    // requires: a time point
    // returns: nothing
    TimePoint(const TimePoint& other) = default;

    // purpose: compares the time between this and another
    //          returns true if the other time point comes after this
    // requires: another time point
    // returns: a boolean value
    bool operator<(const TimePoint& other) const;

    // purpose: compares the time between this and another
    //          returns true if the other time point comes before this
    // requires: another time point
    // returns: a boolean value
    bool operator>(const TimePoint& other) const;

    // purpose: compares the time between this and another
    //          returns true if the other time point is at the same time
    // requires: another time point
    // returns: a boolean value
    bool operator==(const TimePoint& other) const;

    // purpose: compares the time between this and another
    //          returns true if the other time point is not at the same time
    // requires: another time point
    // returns: a boolean value
    bool operator!=(const TimePoint& other) const;

    // purpose: finds the difference between two time points in hours
    // requires: two time points
    // returns: a double, i.e. the difference in hours
    friend double operator-(const TimePoint&, const TimePoint&);

    // purpose: compares the time between this and another
    //          returns true if the other time point comes after
    //          or is at the same time as this
    // requires: another time point
    // returns: a boolean value
    friend bool operator<=(const TimePoint& t, const TimePoint& other);

    // purpose: compares the time between this and another
    //          returns true if the other time point comes before
    //          or is at the same time as this
    // requires: another time point
    // returns: a boolean value
    friend bool operator>=(const TimePoint& t, const TimePoint& other);

    // purpose: writes the time point to an output stream
    // requires: an output stream, e.g. the console
    // returns: the output stream
    friend std::ostream& operator<<
        (std::ostream& os, const TimePoint& p);

    // purpose: gets the total hours since 0000-00-00 00:00
    // requires: nothing
    // returns: a double, i.e. the total hours
    double toTotalHours() const;

    // purpose: determines if a year is a leap year
    // requires: an integer, i.e. a year
    // returns: true or false
    static bool isLeapYear(int year);

    // purpose: finds the number of days in a month
    // requires: a month and a year
    // returns: an integer
    static int daysInMonth(Months month, int year);

};

using Months = TimePoint::Months;


    /*******************\
    *  month operators  *
    \*******************/

// written by DeepSeek
Months& operator++(Months& month)
{
    if (month == Months::December)
        month = Months::January;
    else
        month = static_cast<Months>(static_cast<int>(month) + 1);

    return month;
}

// moves to the previous month in the year
Months& operator--(Months& month)
{
    if (month == Months::January)
        month = Months::December;
    else
        month = static_cast<Months>(static_cast<int>(month) - 1);

    return month;
}

// offset a month by a given number of months
Months& operator+=(Months& month, int offset)
{
    month = static_cast<Months>((static_cast<int>(month) + offset - 1) % 12 + 1);

    return month;
}

// written by DeepSeek
// offset a month by a given number of months
Months& operator-=(Months& month, int offset)
{
    month = static_cast<Months>((static_cast<int>(month) - offset - 1) % 12 + 1);
    if (static_cast<int>(month) <= 0) // Ensure positive modulo result
        month = static_cast<Months>(static_cast<int>(month) + 12);
    return month;
}

// written by DeepSeek
// Compute the difference in months between two Months
int operator-(Months a, Months b)
{
    return static_cast<int>(a) - static_cast<int>(b);
}


    /***************************\
    *  time point constructors  *
    \***************************/

// written by DeepSeek
// default constructor
// sets the time to right now
TimePoint::TimePoint()
{
    // Get the current time point
    const auto now = std::chrono::system_clock::now();

    // Convert the time point to a time_t, which represents calendar time
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Convert the time_t to a tm structure for local time using localtime_s
    std::tm now_tm;
    localtime_s(&now_tm, &now_time_t); // Use localtime_s on Windows

    // Extract the year, month, day, hour, and minute from the tm structure
    year = now_tm.tm_year + 1900; // tm_year is years since 1900
    month = static_cast<Months>(now_tm.tm_mon + 1); // tm_mon is 0-based
    day = now_tm.tm_mday;
    hour = now_tm.tm_hour;
    minute = now_tm.tm_min;

}

// co-written by DeepSeek
// Constructor to handle rollover
TimePoint::TimePoint(int y, Months m = January, int d = 1,
    int h = 12, int min = 0) :
    year(y), month(m), day(d), hour(h), minute(min)
{

    // Handle minute rollover
    if (minute >= 60 || minute < 0)
    {
        hour += minute / 60;
        minute %= 60;
    }

    // Handle hour rollover
    if (hour >= 24 || hour < 0)
    {
        day += hour / 24;
        hour %= 24;
    }

    // Handle day rollover
    while (day > daysInMonth(month, year))
    {
        day -= daysInMonth(month, year);
        ++month;
        if (month == January) year++;
    }

    // handle day "rollunder"
    while (day < 0)
    {
        // if we're wrapping around the year
        if (month == January) year--;

        // adjust the month
        --month;

        // adjust the days
        day += daysInMonth(month, year);
    }

}


    /**********************\
    *  time point methods  *
    \**********************/

// written by DeepSeek
// Helper function to convert a TimePoint to total hours since 0000-00-00 00:00
double TimePoint::toTotalHours() const
{
    // Calculate total years since 0000
    double totalYears = year;

    // Calculate total months since January of the current year
    double totalMonths = static_cast<int>(month) - 1;

    // Calculate total days since the first day of the current month
    double totalDays = day - 1;

    // Calculate total hours since midnight of the current day
    double totalHours = hour;

    // Calculate total minutes since the start of the current hour
    double totalMinutes = minute;

    // Convert everything to hours
    double totalHoursSinceEpoch = totalYears * 365.2425 * 24 + // Account for leap years
        totalMonths * (365.2425 / 12.0) * 24 +  // Average days per month
        totalDays * 24 +
        totalHours +
        totalMinutes / 60.0;

    return totalHoursSinceEpoch;
}

// written by DeepSeek
// Helper function to check if a year is a leap year
bool TimePoint::isLeapYear(int year)
{
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    if (year % 4 == 0)
        return true;
    return false;
}

// written by DeepSeek
// Helper function to get the number of days in a month
int TimePoint::daysInMonth(Months month, int year)
{
    switch (month)
    {
    case January:
    case March:
    case May:
    case July:
    case August:
    case October:
    case December:
        return 31;
    case April:
    case June:
    case September:
    case November:
        return 30;
    case February:
        if (isLeapYear(year))
            return 29;
        else
            return 28;
    default:
        return 0; // Should never happen
    }

}


    /************************\
    *  time point operators  *
    \************************/

// less-than operator
bool TimePoint::operator<(const TimePoint& other) const
{
    // if the years aren't the same
    if (year != other.year)
        // compare the years
        return year < other.year;
    // if the months aren't the same
    else if (month != other.month)
        // compare the months
        return month < other.month;
    // if the days aren't the same
    else if (day != other.day)
        // compare the days
        return day < other.day;
    // if the hours aren't the same
    else if (hour != other.hour)
        // compare the hours
        return hour < other.hour;
    // if the minutes aren't the same
    else if (minute != other.minute)
        return minute < other.minute;

    // if we make it through all the if-else statements,
    // the times are the exact same, and therefore not less than
    return false;
}

// greater-than operator
bool TimePoint::operator>(const TimePoint& other) const
{
    // if the years aren't the same
    if (year != other.year)
        // compare the years
        return year > other.year;
    // if the months aren't the same
    else if (month != other.month)
        // compare the months
        return month > other.month;
    // if the days aren't the same
    else if (day != other.day)
        // compare the days
        return day > other.day;
    // if the hours aren't the same
    else if (hour != other.hour)
        // compare the hours
        return hour > other.hour;
    // if the minutes aren't the same
    else if (minute != other.minute)
        return minute > other.minute;

    // if we make it through all the if-else statements,
    // the times are the exact same, and therefore not greater than
    return false;
}

// equal-to operator
bool TimePoint::operator==(const TimePoint& other) const
{
    // if everything is the same
    if (year == other.year && month == other.month && day == other.day
        && hour == other.hour && minute == other.minute)
        // return true
        return true;
    else
        return false;
}

// not-equal-to operator
bool TimePoint::operator!=(const TimePoint& other) const
{
    // if everything is the same
    if (year == other.year && month == other.month && day == other.day
        && hour == other.hour && minute == other.minute)
        // return false
        return false;
    else
        return true;
}

// get the difference in hours between two timepoints
double operator-(const TimePoint& a, const TimePoint& b)
{
    // Convert both TimePoints to total hours since a fixed epoch (e.g., 0000-00-00 00:00)
    auto totalHoursA = a.toTotalHours();
    auto totalHoursB = b.toTotalHours();

    // Return the difference in hours
    return totalHoursA - totalHoursB;
}

// is a timepoint before or at the same time as another?
static bool operator<=(const TimePoint& t, const TimePoint& other)
{
    if (t < other || t == other)
        return true;
    else
        return false;
}

// is a timepoint after or at the same time as another
static bool operator>=(const TimePoint& t, const TimePoint& other)
{
    if (t > other || t == other)
        return true;
    else
        return false;
}

// give the time point the ability to be written
static std::ostream& operator<<(std::ostream& os, const TimePoint& p)
{
    os << std::setw(4) << std::setfill('0') << p.year;

    os << "/";

    os << std::setw(2) << std::setfill('0') << static_cast<int>(p.month);

    os << "/";

    os << std::setw(2) << std::setfill('0') << p.day;

    os << " ";

    os << std::setw(2) << std::setfill('0') << p.hour;
    os << std::setw(2) << std::setfill('0') << p.minute;

    return os;
}
