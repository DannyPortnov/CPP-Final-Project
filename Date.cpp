#include "Date.h"

//constructor that creates an instance with the current day
Date::Date() {
	tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	init(newtime.tm_mday, newtime.tm_mon + 1, newtime.tm_year + 1900);
    create_formated_date_string(m_date);
}


//constructor that creates an instance by string
Date::Date(string date) : m_date(date) {
    set_date_from_string(date);
}

// set the date
Date& Date::init(int d, int m, int y) {
	m_day = d; m_month = m; m_year = y;
	return *this;
}

// extract date in format: "d/m/yyyy" to day, month, year
void Date::set_date_from_string(string date) {
    stringstream ss(date);
    int day, month, year;
    char delimiter = '/'; // we do not need to explicitly define the delimiter variable as "/" 
                          // because it is the default delimiter used by the '>>' operator.
    ss >> day >> delimiter >> month >> delimiter >> year;
    init(day, month, year);
}

// change date, return true if date was changed successfully
bool Date::change_date() {
    bool invalid_answer = true;
    while (invalid_answer) {
        cout << "Enter the date in the following format- dd/mm/yyyy: ";
        string answer; cin >> answer; cout << endl;
        if (validate_before_change(answer) == true) {
            set_date_from_string(answer);
            create_formated_date_string(m_date);
            cout << "Date was successfully changed!" << endl;
            return true;
        }
        else {
            bool invalid_char = true;
            while (invalid_char) {
                cout << "Still want to change date? y/n: ";
                char answer; cin >> answer; cout << endl;
                if (answer == 'n') {
                    cout << "Date was not changed." << endl;
                    return false;
                }
                else if (answer == 'y') {
                    cout << "Try again. " << endl;
                    invalid_char = false;
                }
            }
        }
    }
}

//checks if date is valid in order to allow to change it
bool Date::validate_before_change(string date) {
    stringstream ss(date);
    int day, month, year;
    char delimiter = '/';
    ss >> day >> delimiter >> month >> delimiter >> year;
    int max_day = days_in_each_month(is_leap_year(year))[month];
    if (day > max_day) {
        cout << "Invalid date! The maximum number of days of this specified month is:  " << max_day << endl;
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

// creates a string in the format dd/mm/yyyy
void Date::create_formated_date_string(string& date) {
    stringstream ss;
    if (m_month < 10 && m_day < 10)
        ss << '0' << m_day << '/0' << m_month << '/' << m_year;
    else if (m_month < 10)
        ss << m_day << '/0' << m_month << '/' << m_year;
    else if (m_day < 10)
        ss << '0' << m_day << '/' << m_month << '/' << m_year;
    date = ss.str();
}

// returns the date as a string in format dd/mm/yyyy
string& Date::get_date_as_string() {
    return m_date;
}


// assign one date to another
Date& Date::operator=(const Date& otherDate) {
	return init(otherDate.m_day, otherDate.m_month, otherDate.m_year);
}

// operator overload << to show the date
ostream& operator<<(ostream& os, const Date& a) {
    if (a.m_month < 10 && a.m_day < 10)
        os << '0' << a.m_day << '/0' << a.m_month << '/' << a.m_year << endl;
    if (a.m_month < 10)
        os << a.m_day << '/0' << a.m_month << '/' << a.m_year << endl;
    if (a.m_day < 10)
        os << '0' << a.m_day << '/' << a.m_month << '/' << a.m_year << endl;
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