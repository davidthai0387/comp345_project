#include "GameEngine.h"

#include <iostream>
#include <time.h>

using namespace std;

int main() {
    GameEngine ge;
    //When selecting the map here are the different cases
    // The map is in the directory but invalid (solar)
    // The map is in the directory and valid (europe)
    // The map is not in the directory (any other name)
    ge.GameStart();
    cout << "Number of players that were created: " << ge.getPlayersList().size() << endl;
    cout << "Size of deck: " << (*(ge.getDeck())).getDeck().size() << endl;

    // You set the initial observer status using a prompt from the GameStart() method
    /** After the user sets the status of the observers, we will change the status two times
     * to show the effectiveness of our methods**/
    if(ge.getObserverStatus()) {
        cout << "Observers are on" << endl;
        ge.setObserverStatus(false);
    }
    else {
        cout << "Observers are off" << endl;
        ge.setObserverStatus(true);
    }

    if(ge.getObserverStatus()) {
        cout << "Observers are on" << endl;
        ge.setObserverStatus(false);
    }
    else {
        cout << "Observers are off" << endl;
        ge.setObserverStatus(true);
    }

    if(ge.getObserverStatus()) 
        cout << "Observers are on" << endl;
    else
        cout << "Observers are off" << endl;

    ge.mainGameLoop();
    return 0;
}