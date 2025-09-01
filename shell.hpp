#pragma once


// includes

#include <algorithm>
#include "event/event.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "json.hpp"
#include <queue>
#include <vector>


// usings

using json = nlohmann::json;
using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ostream;
using std::string;
using std::queue;
using std::vector;


// prototypes

// purpose: prints every element of a heap
// requires: a heap
// returns: nothing, but prints to the console
template <typename adt>
void printHeap(vector<adt>&);


    /*********\
    *  Shell  *
    \*********/

// allows the interaction between the user and a .json file
class Shell
{
public:
    
        /***********************\
        *  default constructor  *
        \***********************/

    // purpose: opens the json file "to_do_list.json", dynamically allocates
    //          the heap, sets the shell running, and prints the command list
    // requires: nothing
    // returns: nothing
    Shell();

    ~Shell();


        /*****************************\
        *  parametrized constructors  *
        \*****************************/

    // purpose: opens the json file, dynamically allocates the heap,
    //          sets the shell running, and prints the command list
    // requires: the name of the file
    // returns: nothing
    Shell(const string);

    // purpose: opens the json file, assigns the heap to the one passed
    //          through, sets the shell running, and prints the command line
    // requires: the file name, and a heap
    // returns: nothing
    Shell(const string, vector<Event>&);

    // purpose: opens the json file, dynamically allocates the heap with the
    //          event passed through, sets the shell running, and prints the
    //          command line
    // requires: the file name, and an event
    // returns: nothing
    Shell(const string fileName, const Event& myEvent);


        /********************\
        *  member functions  *
        \********************/

    // purpose: determines if the shell is running
    // requires: nothing
    // returns true
    bool isRunning() const { return is_running; }

    // purpose: prompts the user for the command
    // requires: nothing
    // returns a string, i.e. the command
    vector<string> getCommand() const;

    // purpose: actually runs the commands
    // requires: a vector of strings, i.e. individual keywords
    // returns:: nothing
    void runCommand(vector<string>);

private:

    bool is_running;
    bool ownsHeap;
    fstream jsonFile;
    vector<Event> toDoList;
    json jsonData;

    // purpose: adds an event to the list
    // requires: an event
    // returns: nothing
    void addEvent(const Event& myEvent)
    {
        toDoList.push_back(myEvent);
        push_heap(toDoList.begin(), toDoList.end());
    }

    // purpose: prints an error to the user saying the shell doesn't understand
    // requires: a string
    // returns: nothing
    void errorNote(string);

    // purpose: searches for an event by asking the user for relevant info
    // requires: optionally a string
    // returns: the index of the event in the heap
    int findJSONEvent(string);

    // purpose: writes each element of the heap to the json file
    // requires: nothing
    // returns: nothing
    void heap2JSON();

    // purpose: takes data from a json file and tosses it onto the heap
    // requires: an nlohmann::json data type
    // returns: nothing
    void JSON2heap();

    // purpose: asks the user about the event, and writes it to the json file
    // requires: nothing
    // returns: a pointer to the event
    Event* makeEvent();

    // purpose: checks if a json entry matches an event
    // requires: a json entry and an event
    // returns: a boolean value
    bool matchesJSONEvent(const json&, const Event&);
    
    // purpose: separates a string using a delimiter
    // requires: a full command, can take in a delimiter
    // returns: a vector of strings
    vector<string> parseCommand(string&, char) const;

    // purpose: prints the events to the console
    // requires: nothing
    // returns: nothing
    void printCommands() const;

    // purpose: prints the event to the console
    // requires: an event
    // returns: nothing
    void printEvent(const Event& event) const { cout << event; }

    // purpose: removes an event from the json data
    // requires: an event
    // returns: nothing
    void removeEventFromJSON(const Event& event);

    // purpose: updates the json file with the json data
    // requires: nothing
    // returns: nothing
    void writeUpdatedJSON();

    // purpose: write an event to the json file
    // requires: an event
    // returns: nothing
    void write2JSON(const Event&);
};


    /****************\
    *  constructors  *
    \****************/

// default constructor
// cowritten by DeepSeek
Shell::Shell() : ownsHeap(true)
{

    // open the file
    jsonFile.open("to_do_list.json", std::ios::in | std::ios::out);

    // Create the file if it doesn't exist
    if (!jsonFile.is_open()) {
        jsonFile.open("to_do_list.json", std::ios::out);
        jsonFile << "{ \"to do list\": [] }"; // Initialize with empty array
        jsonFile.close();
        jsonFile.open("to_do_list.json", std::ios::in | std::ios::out);
    }

    // try to get the json data
    try
    {
        jsonData = json::parse(jsonFile);
    }
    catch (const json::parse_error&) {
        // make it if it doesn't work
        jsonData = json::object();
        jsonData["to do list"] = json::array(); // Ensure it's an array
    }

    /*
    // initialize the heap
    toDoList = new vector<Event>();
    */

    // toss the json values onto the heap
    JSON2heap();

    // set the shell running
    is_running = true;

    printCommands();
}

// parametrized constructor
// cowritten by DeepSeek
// takes in a file name
Shell::Shell(string fileName) : ownsHeap(true)
{

    // open the file
    jsonFile.open(fileName, std::ios::in | std::ios::out);

    // Create the file if it doesn't exist
    if (!jsonFile.is_open()) {
        jsonFile.open(fileName, std::ios::out);
        jsonFile << "{ \"to do list\": [] }"; // Initialize with empty array
        jsonFile.close();
        jsonFile.open(fileName, std::ios::in | std::ios::out);
    }

    // try to get the json data
    try
    {
        jsonData = json::parse(jsonFile);
    }
    catch (const json::parse_error&) {
        // make it if it doesn't work
        jsonData = json::object();
        jsonData["to do list"] = json::array(); // Ensure it's an array
    }

    /*
    // initialize the heap
    toDoList = new vector<Event>();
    */

    // write the json data to the heap
    JSON2heap();
    
    // set the shell running
    is_running = true;

    // print the commands to the console
    printCommands();
}

// parametrized constructor
// cowritten by DeepSeek
// takes in a file name and a heap
Shell::Shell(string fileName, vector<Event>& myHeap) : ownsHeap(false)
{
    // open the file
    jsonFile.open(fileName, std::ios::in | std::ios::out);

    // Create the file if it doesn't exist
    if (!jsonFile.is_open()) {
        jsonFile.open(fileName, std::ios::out);
        jsonFile << "{ \"to do list\": [] }"; // Initialize with empty array
        jsonFile.close();
        jsonFile.open(fileName, std::ios::in | std::ios::out);
    }

    // try to get the json data
    try
    {
        jsonData = json::parse(jsonFile);
    }
    catch (const json::parse_error&) {
        // make it if it doesn't work
        jsonData = json::object();
        jsonData["to do list"] = json::array(); // Ensure it's an array
    }

    // set the to do list to the heap passed through
    toDoList = myHeap;

    // toss all the events already in the json file to the heap
    // since the heap has duplicate checking, this is fine
    JSON2heap();

    // clear the json file
    jsonFile.close();
    jsonFile.open("to_do_list.json", std::ios::out | std::ios::trunc);  // Truncate the file
    jsonData["to do list"] = json::array();  // Reset in-memory JSON to empty array
    jsonFile << std::setw(4) << jsonData;    // Write empty array to file
    jsonFile.close();
    jsonFile.open("to_do_list.json", std::ios::in | std::ios::out);  // Reopen for operations

    // write each event in the heap to the json file
    heap2JSON();

    // set the shell running
    is_running = true;
    
    // print the command list
    printCommands();
}

// parametrized constructor
// takes in a file name and an event
Shell::Shell(string fileName, const Event& myEvent) : ownsHeap(true)
{
    is_running = true;

    jsonFile.open(fileName);

    jsonData = json::parse(jsonFile);

    toDoList.push_back(myEvent);

    printCommands();
}

// destructor
Shell::~Shell()
{
    jsonFile.close();
    if (ownsHeap)
        toDoList.erase(toDoList.begin(), toDoList.end());
}


    /********************\
    *  member functions  *
    \********************/

        /* public */

// get the command (a string) from the user
vector<string> Shell::getCommand() const
{
    string command;

    cout << endl << ">> ";
    getline(cin, command);

    cout << "\n";

    return parseCommand(command, ' ');
}

// run the command line that is passed through
void Shell::runCommand(vector<string> cmd_line)
{
    int kw_count = 0;
    // keywords from the command line
    string keyword = cmd_line[kw_count];

    try
    {
        // if it's help
        if (keyword == "help")
            // print the commands
            printCommands();

        // if the keyword is quit
        else if (keyword == "quit")
            // turn the shell off
            is_running = false;

        // if the keyword is make
        else if (keyword == "make")
        {
            keyword = cmd_line[++kw_count];

            // if the user wants to make an event
            if (keyword == "event")
            {
                // make one!
                Event* nEvent = makeEvent();

                // add it to the heap
                toDoList.push_back(*nEvent);
                make_heap(toDoList.begin(), toDoList.end());

                // write it to the json file
                write2JSON(*nEvent);
            }
            else
            {
                errorNote(keyword);
            }

        }

        // if the keyword is view
        else if (keyword == "view")
        {
            int index;
            keyword = cmd_line[++kw_count];

            // look at the whole list
            if (keyword == "list")
            {
                for (auto &i : toDoList)
                    cout << i << '\n';
            }
            // or just the top
            else if (keyword == "top")
            {
                cout << toDoList[0];
            }
            // or let the user pick an event
            else if (keyword == "event")
            {
                // get the new keyword
                keyword = cmd_line[++kw_count];

                try
                {
                    index = stoi(keyword);

                    cout << toDoList[index];
                }
                catch (const std::invalid_argument& e)
                {
                    index = findJSONEvent(keyword);

                    cout << e.what() << '\n';
                    if (index == -1)
                    {
                        cout << "Event not found" << endl;
                    }
                    else
                    {
                        cout << toDoList[index];
                    }
                }
                catch (const std::exception& e)
                {
                    cout << e.what() << endl;
                }

            }
            else
            {
                errorNote(keyword);
            }

        }

        // if the keyword is finish
        else if (keyword == "finish")
        {
            keyword = cmd_line[++kw_count];

            if (keyword == "top")
            {
                Event topEvent = toDoList[0];
                pop_heap(toDoList.begin(), toDoList.end());

                removeEventFromJSON(topEvent);
            }
            else
            {
                errorNote(keyword);
            }

        }

        // if the user enters an unknown keyword
        else
        {
            errorNote(keyword);
        }

    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }

    return;
}

        /* private */

// print an error note to the console
void Shell::errorNote(string keyword)
{
    cout << "\"" << keyword << "\" unknown\n\n";

    printCommands();

}

// write every event of the heap to the json file
void Shell::heap2JSON()
{
    for (size_t i = 0; i < toDoList.size(); i++)
    {
        write2JSON(toDoList[i]);
    }

}

// find the index of the event in the heap with the user's help
int Shell::findJSONEvent(string keyword = "")
{
    char firstChar = keyword[0];
    size_t choice = 0;
    string title;
    vector<int> indices;

    if (keyword == "")
    {
        // get the event name from the user
        cout << "Event name:" << endl << ">> ";
        getline(cin, title);

        // go through the to do list
        for (size_t i = 0; i < toDoList.size(); i++)
        {
            // mark every event that has the same name as the title
            if (toDoList[i].get_name() == title)
            {
                indices.push_back(i);
            }

        }

        // if there are no events
        if (indices.size() == 0)
        {
            return -1;
        }
        // if there are multiple events with the same name
        if (indices.size() != 1)
        {
            cout << "Multiple events exist with the name \"" << title << "\"\n";

            do
            {
                // list every event with the same name
                for (size_t i = 0; i < indices.size(); ++i)
                {
                    cout << i + 1 << "\n";
                    cout << toDoList[indices[i]].get_name() << "\n\t";
                    cout << toDoList[indices[i]].get_description();
                    cout << "\n";
                }

                // prompt the user for the event
                cout << "Choose the event\n>> ";
                cin >> choice;

                // if the choice is invalid
                if (choice <= 0 || choice > indices.size())
                {
                    cout << "\nInvalid input\n";
                }

                cin.ignore();

                // repeat
            } while (choice <= 0 || choice > indices.size());

            // return the index and make sure there's no overflow
            return indices[choice - 1];
        }

        return indices[0];
    }
    else
        return 256;
}

// pushes all the json data onto the heap
void Shell::JSON2heap()
{
    for (auto &i : jsonData["to do list"])
    {
        Event* temp;

        // get and set the time
        TimePoint* time = new TimePoint(i["due_date"]["year"],
            stom(i["due_date"]["month"]), i["due_date"]["day"],
            i["due_date"]["hour"], i["due_date"]["minute"]);

        // make the event
        temp = new Event( i["title"], *time,
            static_cast<unsigned short>( i["importance"] ),
            static_cast<double>( i["bias"] ), i["description"]);

        // toss the event on the to do list
        toDoList.push_back(*temp);
        
    }
    push_heap(toDoList.begin(), toDoList.end());
}

// prompts the user for event info and returns such an event
Event* Shell::makeEvent()
{
    double bias;
    Event* nEvent;
    int days;
    int hour;
    int minute;
    int year;
    Months month;
    string title;
    TimePoint* time;
    unsigned short importance;

    // ask the user for the relevant info
    readEventInfo(title, year, month, days, hour, minute, importance, bias);

    // create the time
    time = new TimePoint(year, month, days, hour, minute);

    // create the event
    nEvent = new Event(title, *time, importance, bias);

    return nEvent;
}

// written by DeepSeek
bool Shell::matchesJSONEvent(const json& eventJson, const Event& event) {
    // Compare all fields
    return (
        eventJson["title"] == event.get_name() &&
        eventJson["importance"] == event.get_importance() &&
        eventJson["bias"] == event.get_bias() &&
        eventJson["due_date"]["year"] == event.get_due_date().year &&
        eventJson["due_date"]["month"] == mtos(event.get_due_date().month) &&
        eventJson["due_date"]["day"] == event.get_due_date().day &&
        eventJson["due_date"]["hour"] == event.get_due_date().hour &&
        eventJson["due_date"]["minute"] == event.get_due_date().minute
        );
}

// parses the command
vector<string> Shell::parseCommand(string& command, char del = ' ') const
{
    vector<string> cmd_line;
    string keyWord = "";

    // add the delimiter at the end of the command to ensure proper runniage
    command += del;

    // iterate through every char of command
    for (auto &i : command)
    {
        // if we're at the delimiter
        if (i == del)
        {
            // push the key word onto the command line
            cmd_line.push_back(keyWord);
            // reset the key word
            keyWord = "";
        }
        else
        {
            // push the chars onto the string
            keyWord.push_back(i);
        }

    }

    return cmd_line;
}

// print the commands to the console
void Shell::printCommands() const
{
    cout << "\t-- Commands --\n";
    cout << "help\n\tPrints the command list\n";
    cout << "quit\n\tTurn off the shell\n";
    cout << "make [object]\n\tMake the object passed through\n";
    cout << "view [object]\n\tView he object passed through\n";
    cout << "finish [object]\n\tRemove the object from the list\n";
    
    cout << "\n\t-- Objects --\n";
    cout << "list\n\tThe To Do List itself\n";
    cout << "top\n\tThe most important Event in the list\n";
    cout << "event\n\tA specific Event\n";
}

// written by DeepSeek
// removes an event from the json data
void Shell::removeEventFromJSON(const Event& event) {
    // Get the "to do list" array reference
    auto& todoArray = jsonData["to do list"];

    // Iterate through the JSON array
    for (auto it = todoArray.begin(); it != todoArray.end(); ++it) {
        if (matchesJSONEvent(*it, event)) {
            // Erase the matching element
            todoArray.erase(it);
            break; // Exit after first match
        }
    }

    // Write changes back to the file
    writeUpdatedJSON();
}

// written by DeepSeek
// writes the json data to the file
void Shell::writeUpdatedJSON() {
    jsonFile.close();
    jsonFile.open("to_do_list.json", std::ios::out | std::ios::trunc);
    jsonFile << std::setw(4) << jsonData << endl;
    jsonFile.close();
    jsonFile.open("to_do_list.json", std::ios::in | std::ios::out);
}

// cowritten by DeepSeek
// write an event to the json file
void Shell::write2JSON(const Event& event)
{
    json eventJson;
    eventJson["title"] = event.get_name();
    eventJson["importance"] = event.get_importance();
    eventJson["bias"] = event.get_bias();
    eventJson["description"] = event.get_description();

    TimePoint dueDate = event.get_due_date();
    eventJson["due_date"]["year"] = dueDate.year;
    eventJson["due_date"]["month"] = mtos(dueDate.month);
    eventJson["due_date"]["day"] = dueDate.day;
    eventJson["due_date"]["hour"] = dueDate.hour;
    eventJson["due_date"]["minute"] = dueDate.minute;

    // Append the new event to the "to do list" array
    jsonData["to do list"].push_back(eventJson);

    // Write the updated JSON back to the file
    jsonFile.close();
    jsonFile.open("to_do_list.json", std::ios::out | std::ios::trunc);
    jsonFile << std::setw(4) << jsonData << endl;
    jsonFile.close();
    // Reopen for subsequent operations
    jsonFile.open("to_do_list.json", std::ios::in | std::ios::out);
}


    /********************\
    *  global functions  *
    \********************/

// print every value from the heap
template <typename adt>
void printHeap(vector<adt>& heap)
{
    int heapSize = heap.getSize();
    queue<adt> tempq;

    // go through every element in the heap
    for (int i = 0; i < heapSize; i++)
    {
        // pop the top of the heap off and push it onto the queue
        tempq.push(heap.pop());
    }

    while (!tempq.empty())
    {
        // print the front element
        cout << tempq.front() << ((tempq.size() != 1) ? "\n" : "");
        // toss it onto the heap
        heap.toss(tempq.front());
        // pop it off the queue
        tempq.pop();
    }

}
