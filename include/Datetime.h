#ifndef DATETIME_H
#define DATETIME_H

#include <iostream>
#include <stdexcept>
#include <string.h>
#include <sstream>
#include <chrono>
class Date {
protected:
    int year, month, day;

    // Helper function to check if a year is a leap year
    bool isLeapYear(int year) const;

    // Helper function to get the number of days in a month
    int daysInMonth(int month, int year) const;

    // Helper function to validate and normalize the date
    void validateDate();

public:
    // Constructors
    Date(int year = 1970, int month = 1, int day = 1);

    static Date now(){
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return Date(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    }
    std::string tostring() const;
    // Operators
    Date& operator+=(int days);
    Date& operator-=(int days);
    Date& operator++();    // Prefix ++
    Date& operator--();    // Prefix --
    int operator-(const Date& other) const;

    // Friend functions
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    friend std::istream& operator>>(std::istream& is, Date& date);
    // Set date 
    Date& operator=(std::string dateStr);
    Date& operator=(const Date& other);
};

class DateTime : public Date {
private:
    int hour, minute, second;

    // Helper function to validate and normalize the time
    void validateTime();

public:
    // Constructors
    DateTime(int year = 1970, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0);
    // Get time at the moment
    static DateTime now(){
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return DateTime(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    }
    // Operators
    DateTime& operator+=(long seconds);
    DateTime& operator-=(long seconds);
    DateTime& operator++();    // Prefix ++ (increment by 1 second)
    DateTime& operator--();    // Prefix -- (decrement by 1 second)
    int operator-(const DateTime& other) const;
    // Format to string
    std::string tostring() const;
    // Friend functions
    friend std::ostream& operator<<(std::ostream& os, const DateTime& dt);
    friend std::istream& operator>>(std::istream& is, DateTime& dt);
    // Set datetime 
    DateTime& operator= (std::string dateTimeStr);
    DateTime& operator= (const DateTime& other);
};

#endif // DATETIME_H
