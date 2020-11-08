#include <iostream>
#include <fstream>
#include <vector>
#include "GameEngine.h"

using namespace std;

GameEngine::GameEngine() {
    nbOfPlayers = 0;
    Deck deckCards;
    activateObservers = true;
}

bool GameEngine::equals(const string& a, const string& b) {
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

void GameEngine::GameStart() {
    string map;
    string mapName = selectMap();
    while(mapName.compare("") == 0) {
        cout << "The map that you've selected does not exist in this current directory. You will be asked to select another one." << endl;
        mapName = selectMap();
    }

    MapLoader ml(mapName);
    vector<string> mapText = ml.read();
    while(!ml.checkFormat(mapText)) {
        cout << "The map that you've selected has been deemed as invalid. You will be asked to select another one." << endl;
        mapName = selectMap();
        while(mapName.compare("") == 0) {
            cout << "The map that you've selected does not exist in this current directory. You will be asked to select another one." << endl;
            mapName = selectMap();
        }
        MapLoader ml(mapName);
        mapText = ml.read();
    }
    cout << endl;
    vector<tuple<string, int>> continents = ml.parseContinents(mapText[2]);
    vector<tuple<string, int>> countries = ml.parseCountries(mapText[3]);
    vector<vector<int>> borders = ml.parseBorders(mapText[4]);
    Map m(continents, ml.getNumOfContinents(), countries, ml.getNumOfCountries(),borders);
    gameMap = m;
    gameMap.validate();
    
    setNbOfPlayers();
    activateObservers = Observers();
    cout << endl;
    for (int i = 0; i < nbOfPlayers; i++) {
        string name;
        cout << "Enter the name for player #" << (i+1) << ": ";
        cin >> name;
        Player newPlayer(name);
        players.push_back(newPlayer);
    }
    cout << endl;
}

string GameEngine::selectMap() {
    string map;
    cout << "What map would you like to play with ?: ";
    cin >> map;
    if(isMapInDirectory(map + ".map"))
        return map + ".map";
    else
        return "";    
}

bool GameEngine::isMapInDirectory(string fileName) {
    ifstream file(fileName);
    if(!file)            
        return false;    
    else                 
        return true;
}

void GameEngine::setNbOfPlayers() {
    int count = 0;
    cout << "Select the number of players that will be participation (Must be between 2 and 5): ";
    cin >> count;
    while(!(count >= 2 && count <=5)) {
       cout << "The numbers of players that you've selected has been deemed invalid. Please pick again: ";
       cin >> count;
    }
    nbOfPlayers = count;
}

bool GameEngine::Observers() {
    string answer;
    bool loopAgain;
    cout << "Would you like to activate the observers for this game? (Yes or No): ";
    cin >> answer;
    while(!equals(answer,"yes") && !equals(answer,"no")) {
       cout << "Your answer has been deeemd invalid. Please enter again: ";
       cin >> answer;
    }
    if(equals(answer,"yes")) 
        return true;
    else if(equals(answer,"no"))
        return false;
    return false;
}

int GameEngine::getNbOfPlayers() {
    return nbOfPlayers;
}

Deck GameEngine::getDeckCards() {
    return deckCards;
}

vector<Player> GameEngine::getPlayersList() {
    return players;
}

bool GameEngine::getObserverStatus() {
    return activateObservers;
}

void GameEngine::setObserverStatus(bool status) {
    activateObservers = status;
}

Map GameEngine::getMap() {
    return gameMap;
}