#ifndef _DATE_H_
#define _DATE_H_
#include <iostream>
#include <time.h>
#include <sstream>
#include <string>
#include <regex>
using namespace std;

class Date
{
friend ostream& operator<<(ostream& os, const Date& a); 
friend bool operator<(const Date& a, const Date& b);
private:
	int m_day, m_month, m_year;
	string m_date; // store the date as a string.
	Date& init(int d, int m, int y); // set the date
	void set_date_from_string(string date);
	//works
	void create_formated_date_string(string& date);
	//works
	bool is_date_valid(string date);
public:
	//works
	Date(); //constructor that creates an instance with the current day
	//works
	Date(string date); //constructor that creates an instance by string
	void set_date(); // changes the date, the user have an option to do so.
	string& get_date_as_string(); // in order to put date back in serialization.
	Date& operator=(const Date& otherDate); // assign one date to another
};

ostream& operator<<(ostream& os, const Date& a); // operator overload << to show the date
bool operator<(const Date& date1, const Date& date2); // compare between two dates
//works
int* days_in_each_month(bool is_leap_year);
//works
bool is_leap_year(int year);



#endif		// _DATE_H_