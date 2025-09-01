#pragma once


// includes

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include "timepoint.hpp"


//usings

using std::cin;
using std::cout;
using std::string;


    /*************\ 
    *  constants  *
    \*************/

// the time at compile time
const TimePoint NOW;

// a constant used to scale the priority of an event
constexpr double PRIORITY_CONST = 896.0;


    /**************\
    *  prototypes  *
    \**************/

// purpose: gets the year, month, day, hour, and minute from a string
//          (all integer format)
// requires: a string representing the timepoint,
//           and 5 ints to store the values
// returns: the year, month, day, hour, and minute in integer format
void formatDate(const string&, int&, int&, int&, int&, int&);

// purpose: reads all the info necessary to create an event from the console   
// requires: a string, 5 ints, a month, and a double
// returns: the values of each parameter
void readEventInfo(string&, int&, Months&,
    int&, int&, int&, unsigned short&, double&);

// purpose: converts a string to month
// requires: a string
// returns: a month
Months stom(std::string);

// purpose: converts a month to a string
// requires: a month
// returns: a string
string mtos(Months month);


    /*********\
    *  Event  *
    \*********/

// an event holds the title, due date, importance, and user-imposed bias
// will automatically calculate the priority of itself
class Event
{
public:

    /****************\
    *  constructors  *
    \****************/

    // default constructor
    Event();

    // parametrized constructor
    Event(string, const TimePoint&, unsigned short, double, string);

    // copy constructor
    Event(const Event&);

    ~Event();

    /**********************\
    *  accessor functions  *
    \**********************/

    // get the user-imposed bias
    double get_bias() const { return bias; }

    // get the description of the event
    string get_description() const { return description; }

    // get the base importance of the event
    int get_importance() const { return importance; }

    // get the name of the event
    string get_name() const { return title; }

    // get the due date of the event
    TimePoint get_due_date() const { return *dueDate; }


    /*********************\
    *  mutator functions  *
    \*********************/

    // change the bias
    void set_bias(double nb) { bias = nb; }

    // update the event's due date
    void set_due_date(TimePoint& ndd) { dueDate = &ndd; }

    // change the base importance of the event
    void set_importance(int i) { importance = i; }

    // change the name of the event
    void set_name(string name) { title = name; }


    /************************\
    *  overloaded operators  *
    \************************/

    // purpose: finds out if a < b based on priority levels
    // requires: two events
    // returns: a boolean value
    friend bool operator<(const Event&, const Event&);

    // purpose: finds out if a > b based on priority levels
    // requires: two events
    // returns: a boolean value
    friend bool operator>(const Event&, const Event&);

    // purpose: finds out if a == b based on event data
    // requires: two events
    // returns: a boolean value
    friend bool operator==(const Event&, const Event&);

    // purpose: finds out if a != b based on event data
    // requires: two events
    // returns: a boolean
    friend bool operator!=(const Event&, const Event&);

    // purpose: find out if a <= b based on priority levels
    // requires: two events
    // returns: a boolean
    friend bool operator<=(const Event& a, const Event& b) { return !(a > b); }

    // purpose: find out if a >= b based on priority levels
    // requires: two events
    // returns: a boolean
    friend bool operator>=(const Event& a, const Event& b) { return !(a < b); }

    // purpose: prints the event to the ostream
    // requires: an output stream and an event
    // returns: the output stream
    friend std::ostream& operator<<(std::ostream& os, const Event& thing);

    // purpose: copy assignment for an event
    // requires: another event
    // returns: an event
    Event& operator=(const Event& other);

private:
    unsigned short importance;
    double bias;
    string description;
    string title;
    TimePoint* dueDate;

    // purpose: determines the priority of the event
    // requires: nothing
    // returns: a double
    double get_priority() const;

};


    /**********************\
    *  Event constructors  *
    \**********************/

// default constructor
Event::Event()
{
    // normal importance
    importance = 1;
    // no bias
    bias = 0.0;
    // blank title
    title = "";
    // right now
    dueDate = new TimePoint();
    // blank description
    description = "";
}

// parametrized constructor
Event::Event(string t, const TimePoint& d, unsigned short i = 1,
    double b = 0.0, string des = "") : title(t), dueDate(new TimePoint(d)),
    importance(i), bias(b), description(des) { }

// copy constructor
// default setup for most
Event::Event(const Event& other) : title(other.title),
importance(other.importance), bias(other.bias), description(other.description)
{
    // set up the due date
    dueDate = new TimePoint(*other.dueDate);
}

// destructor
Event::~Event()
{
    delete dueDate;
}


    /*******************\
    *  Event operators  *
    \*******************/

// find out if a < b based on priority levels
bool operator<(const Event& a, const Event& b)
{
    // if a is more important
    if (a.get_priority() > b.get_priority())
        return false;
    // if a is less important
    else if (a.get_priority() < b.get_priority())
        return true;
    // if the priorities are the same
    // compare by the due dates
    else if (a.get_due_date() > b.get_due_date())
        // a is less important if it comes later 
        return true;
    // if a doesn't come after b, check if it comes before
    else if (a.get_due_date() < b.get_due_date())
        // a is more important if it comes before b
        return false;
    // if, by some miracle they're still the same priority
    else
        // compare by name
        return a.get_name() < b.get_name();
}

// find out if a > b based on priority
bool operator>(const Event& a, const Event& b)
{
    // if a is more important
    if (a.get_priority() < b.get_priority())
        return false;
    // if a is less important
    else if (a.get_priority() > b.get_priority())
        return true;
    // if the priorities are the same
    // compare by the due dates
    else if (a.get_due_date() < b.get_due_date())
        // a is less important if it comes later 
        return true;
    // if a doesn't come after b, check if it comes before
    else if (a.get_due_date() > b.get_due_date())
        // a is more important if it comes before b
        return false;
    // if, by some miracle they're still the same priority
    else
        // compare by name
        return a.get_name() > b.get_name();
}

// find out if a == b based on event data
bool operator==(const Event& a, const Event& b)
{
    // if a and b don't have the same due date
    if (a.get_due_date() != b.get_due_date())
        return false;
    // if the biases are not the same
    else if (a.get_bias() != b.get_bias())
        return false;
    // if a does not have the same importance as b
    else if (a.get_importance() != b.get_importance())
        return false;
    // if the names are not the same
    else if (a.get_name() != b.get_name())
        return false;
    else if (a.get_description() != b.get_description())
        return false;
    else
        return true;
}

// find out if a != b based on event data
bool operator!=(const Event& a, const Event& b)
{
    // if a and b are equally important
    if (a.get_bias() != b.get_bias())
        return true;
    // if the priorities are the same
    // compare by the due dates
    else if (a.get_due_date() != b.get_due_date())
        // a is less important if it comes later 
        return true;
    // if a doesn't come after b, check if it comes before
    else if (a.get_importance() != b.get_importance())
        // a is more important if it comes before b
        return true;
    // if, by some miracle they're still the same priority
    else if (a.get_name() != b.get_name())
        return true;
    else if (a.get_description() != b.get_description())
        return true;
    else
        return false;
}

// copy operator
// written by DeepSeek
Event& Event::operator=(const Event& other)
{
    if (this != &other)
    {
        title = other.title;
        importance = other.importance;
        bias = other.bias;
        description = other.description;
        delete dueDate;
        dueDate = new TimePoint(*other.dueDate);
    }
    return *this;
}

// print the event to the ostream
std::ostream& operator<<(std::ostream& os, const Event& thing)
{
    os << thing.get_name();
    if(thing.get_description() != "")
        os << "\n\t" << thing.get_description();
    os << "\nDue by " << thing.get_due_date();
    os << "\nImportance: " << thing.get_importance();
    os << "\nBias: " << thing.get_bias();
    os << "\nPriority Level: " << thing.get_priority() << "\n";

    return os;
}


    /***************************\
    *  event private functions  *
    \***************************/

// get the priority of the event
double Event::get_priority() const
{
    /*
    \frac{ importance }
    { ln( \frac{ e^{bias} + e^{time until due date} + 1 } { e^{bias} + 1 } ) }
    */

    // calculate the denominator
    double den = std::exp(bias) + std::exp(*dueDate - NOW) + 1;
    den /= (std::exp(bias) + 1);
    den = std::log(den);

    // this is the equation that determines priority
    return PRIORITY_CONST * importance / den;
}


    /***************\
    *  definitions  *
    \***************/

// format the date from a string
void formatDate(const string& date, int& y, int& mon, int& d, int& h, int& min)
{
    string year = date.substr(0, 4);
    string month = date.substr(5, 7);
    string day = date.substr(8, 10);
    string time = date.substr(11);
}

// read the event's info from the console
void readEventInfo(string& title, int& year, TimePoint::Months& month, int& day, int& hour,
    int& minute, unsigned short& importance, double& bias)
{
    string date_elements[5];
    string input = "\n>> ";
    string time;

    cout << "Event name" << input;
    getline(cin, title);

    cout << "Due Date\nMM DD YYYY HH MM (military time)" << input;
    getline(cin, time);

    int element = 0;
    for (size_t i = 0; i < time.size(); i++)
    {
        if (time[i] == ' ' || time[i] == '/')
        {
            element++;
        }
        else
        {
            date_elements[element].push_back(time[i]);
        }

    }

    year = stoi(date_elements[2]);
    month = static_cast<TimePoint::Months>(stoi(date_elements[0]));
    day = stoi(date_elements[1]);
    hour = stoi(date_elements[3]);
    minute = stoi(date_elements[4]);

    cout << "Importance of the event (an integer, ideally from 1-3)" << input;
    cin >> importance;

    cout << "User imposed bias: " << input;
    cin >> bias;

    cin.ignore();

}

// string to month
Months stom(std::string month)
{

    if (month == "January")
        return Months::January;
    else if (month == "February")
        return Months::February;
    else if (month == "March")
        return Months::March;
    else if (month == "April")
        return Months::April;
    else if (month == "May")
        return Months::May;
    else if (month == "June")
        return Months::June;
    else if (month == "July")
        return Months::July;
    else if (month == "August")
        return Months::August;
    else if (month == "September")
        return Months::September;
    else if (month == "October")
        return Months::October;
    else if (month == "November")
        return Months::November;
    else if (month == "December")
        return Months::December;
    else
        throw std::invalid_argument("Not a month passed through string-to-month");
}

// month to string
string mtos(Months month)
{
    switch (month)
    {
    case Months::January:
        return "January";
    case Months::February:
        return "February";
    case Months::March:
        return "March";
    case Months::April:
        return "April";
    case Months::May:
        return "May";
    case Months::June:
        return "June";
    case Months::July:
        return "July";
    case Months::August:
        return "August";
    case Months::September:
        return "September";
    case Months::October:
        return "October";
    case Months::November:
        return "November";
    case Months::December:
        return "December";
    default:
        return "";
    }

}
