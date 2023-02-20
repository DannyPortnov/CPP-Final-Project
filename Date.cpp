#include "Date.h"

//constructor that creates an instance with the current day
Date::Date() {
	tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	init(newtime.tm_mday, newtime.tm_mon + 1, newtime.tm_year + 1900);
}

// set the date
Date& Date::init(char d, char m, int y) {
	m_day = d; m_month = m; m_year = y;
	return *this;
}


// assign one date to another
Date& Date::operator=(const Date& otherDate) {
	return init(otherDate.m_day, otherDate.m_month, otherDate.m_year);
}

// operator overload << to show the date
ostream& operator<<(ostream& os, const Date& a) {
	os << (int)a.m_day << "/" << (int)a.m_month << "/" << a.m_year << endl;
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