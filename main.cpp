
/*****************************************************************************\
*                                                                             *
*                            Auto To-Do List Maker                            *
*                              T Emiah Carpenter                              *
*                                                                             *
\*****************************************************************************/


// headers

#include "event/event.hpp"
#include <fstream>
#include "heap/heap.hpp"
#include "json.hpp"
#include <iomanip>
#include <iostream>
#include "shell.hpp"
#include <string>


// usings

using json = nlohmann::json;
using std::cin;
using std::cout;
using std::fstream;
using std::ifstream;
using std::ofstream;


    /********\
    *  main  *
    \********/

int main()
{
    //ifstream jsonFile;
    //json jsonData;
    Heap<Event> myHeap;
    Shell daShell("to_do_list.json", myHeap);
    vector<string> cmds;

    do
    {
        // get the command
        cmds = daShell.getCommand();

        // run the command
        daShell.runCommand(cmds);

    } while (daShell.isRunning());

    return 0;
}
