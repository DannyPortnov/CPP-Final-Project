#include "Date.h"

//constructor that creates an instance with the current day
Date::Date() {
	tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	init(newtime.tm_mday, newtime.tm_mon + 1, newtime.tm_year + 1900);
    create_formated_date_string(m_date);
    //m_date = 
}

#define empty_date "00/00/1900"

//constructor that creates an instance by std::string
Date::Date(std::string date) {
    if (date.empty()) {
        set_date_from_string(empty_date);
        return;
    }
    if (is_date_valid(date)) {
        set_date_from_string(date);
    }
    else {
        set_date();
    }
}


// set the date
Date& Date::init(int d, int m, int y) {
	m_day = d; m_month = m; m_year = y;
	return *this;
}

// extract date in format: "d/m/yyyy" to day, month, year
void Date::set_date_from_string(std::string date) {
    std::stringstream ss(date);
    int day, month, year;
    char delimiter = '/'; // we do not need to explicitly define the delimiter variable as "/" 
    // because it is the default delimiter used by the '>>' operator.
    ss >> day >> delimiter >> month >> delimiter >> year;
    m_date = date;
    init(day, month, year);
}

// change date, return true if date was changed successfully
void Date::set_date() { //todo: check if works
    bool invalid_answer = true;
    while (invalid_answer) {
        std::cout << "Enter the date in the following format- dd/mm/yyyy: ";
        std::string answer;
        std::cin >> answer; 
        std::cout << std::endl;
        if (is_date_valid(answer)) {
            set_date_from_string(answer);
            create_formated_date_string(m_date);
            std::cout << "Date was successfully set!" << std::endl;
            return;
        }
        else {
            bool invalid_char = true;
            while (invalid_char) {
                std::cout << "Still want to set date? y/n: ";
                char answer; std::cin >> answer; std::cout << std::endl;
                if (answer == 'n') {
                    std::cout << "Date was not set." << std::endl;
                    set_date_from_string(empty_date);
                    return;
                }
                else if (answer == 'y') {
                    std::cout << "Try again. " << std::endl;
                    invalid_char = false;
                }
            }
        }
    }
}

//checks if date is valid in order to allow to change it
bool Date::is_date_valid(std::string date) {
    std::regex pattern("\\d{2}/\\d{2}/\\d{4}");
    bool valid_pattern = regex_match(date, pattern);
    if (valid_pattern == false) {
        std::cout << date << " is not a valid date format" << std::endl;
        return false;
    }
    std::stringstream ss(date);
    int day, month, year;
    char delimiter = '/';
    ss >> day >> delimiter >> month >> delimiter >> year;
    if (month < 0 || month > 12) {
        std::cout << "Invalid date! Month isn't 1-12"<< std::endl;
        return false;
    }
    int max_day = days_in_each_month(is_leap_year(year))[month];
    if (day > max_day || day < 0) {
        std::cout << "Invalid date! Day cannot be bigger than " << max_day << " Or less than 0" << std::endl;
        return false;
    }
    return true;
}


// Checks if the given year is leap year. returns True if year is leap year, False otherwise
bool is_leap_year(int year) {
    // Return true if year is a multiple
    // 0f 4 and not multiple of 100.
    // OR year is multiple of 400.
    return (year >= 0 && (((year % 4 == 0) && (year % 100 != 0)) ||
        (year % 400 == 0)));
}


int* days_in_each_month(bool is_leap_year) {
    int array_of_days[] = { NULL, 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31,30, 31, 30, 31 };
    return array_of_days;
}

// creates a std::string in the format dd/mm/yyyy
void Date::create_formated_date_string(std::string& date) {
    std::stringstream ss;
    if (m_month < 10 && m_day < 10)
        ss << "0" << m_day << "/0" << m_month << "/"  << m_year;
    else if (m_month < 10)
        ss << m_day << "/0" << m_month << "/"  << m_year;
    else if (m_day < 10)
        ss << "0" << m_day << "/" << m_month << "/" << m_year;
    date = ss.str();
}

// returns the date as a std::string in format dd/mm/yyyy
std::string& Date::get_date_as_string() {
    return m_date;
}


// assign one date to another
Date& Date::operator=(const Date& otherDate) {
    m_date = otherDate.m_date; //update the m_date also
	return init(otherDate.m_day, otherDate.m_month, otherDate.m_year);
}

// operator overload << to show the date
std::ostream& operator<<(std::ostream& os, const Date& a) {
    os << a.m_date;
	return os;
}

//returns true if the date1 is before date2, otherwise false.
bool operator<(const Date& date1, const Date& date2) {
    if (date1.m_year < date2.m_year) {
        return true;
    }
    else if (date1.m_year == date2.m_year) {
        if (date1.m_month < date2.m_month) {
            return true;
        }
        else if (date1.m_month == date2.m_month) {
            if (date1.m_day < date2.m_day) {
                return true;
            }
        }
    }
    return false;
}

//returns true if date is the same as date2, otherwise false.
bool Date::operator==(const Date& date2) {
    if (m_date == date2.m_date) {
        return true;
    }
    return false;
}