#include "Include/DateTime.h"

// ------------------------ Date Class ------------------------



bool Date::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int month, int year) const {
    static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

void Date::validateDate() {
    if (year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth(month, year)) {
        throw std::invalid_argument("Invalid date provided.");
    }
}

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {
    validateDate();
}

Date& Date::operator+=(int days) {
    if (days < 0) return *this -= -days;

    day += days;
    while (day > daysInMonth(month, year)) {
        day -= daysInMonth(month, year);
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
    return *this;
}

Date& Date::operator-=(int days) {
    if (days < 0) return *this += -days;

    day -= days;
    while (day < 1) {
        month--;
        if (month < 1) {
            month = 12;
            year--;
        }
        day += daysInMonth(month, year);
    }
    return *this;
}

Date& Date::operator++() {
    return *this += 1;
}

Date& Date::operator--() {
    return *this -= 1;
}

int Date::operator-(const Date& other) const {
    int totalDays1 = year * 365 + day;
    for (int i = 1; i < month; i++) {
        totalDays1 += daysInMonth(i, year);
    }

    int totalDays2 = other.year * 365 + other.day;
    for (int i = 1; i < other.month; i++) {
        totalDays2 += daysInMonth(i, other.year);
    }

    return totalDays1 - totalDays2;
}

// Returns a string in the format "YYYY-MM-DD"
std::string Date::tostring() const {
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << year << "-"
        << std::setw(2) << std::setfill('0') << month << "-"
        << std::setw(2) << std::setfill('0') << day;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os  << date.tostring();
    return os;
}

std::istream& operator>>(std::istream& is, Date& date) {
    char sep;
    is >> date.day >> sep >> date.month >> sep >> date.year;
    date.validateDate();
    return is;
}
Date& Date::operator=(std::string dateStr) {
    int d, m, y;
    char sep1, sep2;
    std::istringstream iss(dateStr);

    iss >> y >> sep1 >> m >> sep2 >> d;
    if (sep1 != '-' || sep2 != '-' || iss.fail()) {
        throw std::invalid_argument("Invalid date format. Expected format: YYYY-MM-DD");
    }

    day = d;
    month = m;
    year = y;
    validateDate();
    return *this;
}

// Overload the assignment operator
Date& Date::operator=(const Date& other) {
    if (this != &other) { // Check for self-assignment
        this->year = other.year;
        this->month = other.month;
        this->day = other.day;
    }
    return *this;
}


// ------------------------ DateTime Class ------------------------

void DateTime::validateTime() {
    if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60 || second < 0 || second >= 60) {
        throw std::invalid_argument("Invalid time provided.");
    }
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
    : Date(year, month, day), hour(hour), minute(minute), second(second) {
    validateDate();
    validateTime();
}

DateTime& DateTime::operator+=(long seconds) {
    if (seconds < 0) return *this -= -seconds;

    second += seconds;
    while (second >= 60) {
        second -= 60;
        minute++;
    }
    while (minute >= 60) {
        minute -= 60;
        hour++;
    }
    while (hour >= 24) {
        hour -= 24;
        Date::operator++();  // Increment the date by 1 day
    }
    return *this;
}

DateTime& DateTime::operator-=(long seconds) {
    if (seconds < 0) return *this += -seconds;

    second -= seconds;
    while (second < 0) {
        second += 60;
        minute--;
    }
    while (minute < 0) {
        minute += 60;
        hour--;
    }
    while (hour < 0) {
        hour += 24;
        Date::operator--();  
    }
    return *this;
}

DateTime& DateTime::operator++() {
    return *this += 1;
}

DateTime& DateTime::operator--() {
    return *this -= 1;
}

int DateTime::operator-(const DateTime& other) const {
    int dateDiff = Date::operator-(other);
    int timeDiff = (hour * 3600 + minute * 60 + second) - (other.hour * 3600 + other.minute * 60 + other.second);
    return dateDiff * 86400 + timeDiff;
}
// Returns a string in the format "YYYY-MM-DD HH:MM:SS"
std::string DateTime::tostring() const {
    std::ostringstream oss;
    if (year!=9999){
        oss << Date::tostring() << " "
            << std::setw(2) << std::setfill('0') << hour << ":"
            << std::setw(2) << std::setfill('0') << minute << ":"
            << std::setw(2) << std::setfill('0') << second;
    }
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const DateTime& dt) {
    os << dt.tostring();
    return os;
}

std::istream& operator>>(std::istream& is, DateTime& dt) {
    char sep;
    is >> static_cast<Date&>(dt) >> dt.hour >> sep >> dt.minute >> sep >> dt.second;
    dt.validateTime();
    return is;
}


DateTime& DateTime::operator=(std::string dateTimeStr) {
    if( dateTimeStr!=""){
        int d, m, y, h, min, s;
        char sep1, sep2, sep3, sep4, sep5;
        std::istringstream iss(dateTimeStr);

        iss >> y >> sep1 >> m >> sep2 >> d >> h >> sep3 >> min >> sep4 >> s;
        if (sep1 != '-' || sep2 != '-' || sep3 != ':' || sep4 != ':' || iss.fail()) {
            throw std::invalid_argument("Invalid datetime format. Expected format: YYYY-MM-DD HH:MM:SS");
        }

        day = d;
        month = m;
        year = y;
        hour = h;
        minute = min;
        second = s;

        validateDate(); 
        validateTime();
    }
    else year=9999;
    return *this;
}

// Overload the assignment operator
DateTime& DateTime::operator=(const DateTime& other) {
    if (this != &other) { // Check for self-assignment
        Date::operator=(other); // Call the base class assignment operator
        this->hour = other.hour;
        this->minute = other.minute;
        this->second = other.second;
    }
    return *this;
}
