#ifndef _DATE_H_
#define _DATE_H_
#include <iostream>
#include <time.h>
using namespace std;

class Date
{
friend ostream& operator<<(ostream& os, const Date& a); 
friend bool operator<(const Date& a, const Date& b);
private:
	char m_day, m_month;
	int m_year;
	Date& init(char d, char m, int y); // set the date
public:
	Date(); //constructor that creates an instance with the current day
	Date(string date); //constructor that creates an instance by string
	Date& operator=(const Date& otherDate); // assign one date to another
};

ostream& operator<<(ostream& os, const Date& a); // operator overload << to show the date
bool operator<(const Date& date1, const Date& date2); // compare between two dates



#endif		// _DATE_H_