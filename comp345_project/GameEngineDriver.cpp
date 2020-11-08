#include <iostream>
#include "GameEngine.h"
using namespace std;

int main() {
    GameEngine ge;
    //When selecting the map here are the different cases
    // The map is in the directory but invalid (solar)
    // The map is in the directory and valid (europe)
    // The map is not in the directory (any other name)
    ge.GameStart();
    cout << "Number of players that were created: " << ge.getPlayersList().size() << endl;
    cout << "Size of deck: " << ge.getDeckCards().getDeck().size() << endl;

    if(ge.getObserverStatus()) 
        cout << "Observers are on" << endl;
    else
        cout << "Observers are off" << endl;

    ge.setObserverStatus(true);
    if(ge.getObserverStatus()) 
        cout << "Observers are on" << endl;
    else
        cout << "Observers are off" << endl;
    ge.setObserverStatus(false);
    if(ge.getObserverStatus()) 
        cout << "Observers are on" << endl;
    else
        cout << "Observers are off" << endl;
    return 0;
}