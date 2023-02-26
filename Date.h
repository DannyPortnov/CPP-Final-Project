#ifndef _DATE_H_
#define _DATE_H_
#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <time.h>
#include <sstream>
#include <string>
#include <regex>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
using namespace std;

class Date
{
friend ostream& operator<<(ostream& os, const Date& a); 
friend bool operator<(const Date& a, const Date& b);
private:
	int m_day, m_month, m_year;
	std::string m_date; // store the date as a std::string.
	Date& init(int d, int m, int y); // set the date
	void set_date_from_string(std::string date);
	//works
	void create_formated_date_string(std::string& date);
	//works
	bool is_date_valid(std::string date);
public:
	//works
	Date(); //constructor that creates an instance with the current day
	//works
	Date(std::string date); //constructor that creates an instance by std::string
	void set_date(); // changes the date, the user have an option to do so.
	std::string& get_date_as_string(); // in order to put date back in serialization.
	Date& operator=(const Date& otherDate); // assign one date to another
};

ostream& operator<<(ostream& os, const Date& a); // operator overload << to show the date
bool operator<(const Date& date1, const Date& date2); // compare between two dates
//works
int* days_in_each_month(bool is_leap_year);
//works
bool is_leap_year(int year);



#endif		// _DATE_H_