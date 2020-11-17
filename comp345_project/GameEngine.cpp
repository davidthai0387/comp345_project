#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>
#include "GameEngine.h"
using namespace std;

GameEngine::GameEngine() {
    nbOfPlayers = 0;
    deckCards = new Deck();
    activateObservers = true;
}

bool GameEngine::equals(const string& a, const string& b) {
    int sz = a.size();
    if (b.size() != sz)
        return false;
    for (int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

void GameEngine::GameStart() {
    bool mapIsValid = false;
    do {
        string mapName = selectMap();

        // checking if map file exists
        if (mapName.compare("") == 0) {
            cout << "The map that you've selected does not exist in this current directory. You will be asked to select another one." << endl;
            continue;
        }

        // checking map file format
        MapLoader ml(mapName);
        vector<string> mapText = ml.read();
        if (!ml.checkFormat(mapText)) {
            cout << "The map that you've selected is incorrectly formatted. You will be asked to select another one." << endl;
            continue;
        }

        // checking if map is valid
        vector<tuple<string, int>> continents = ml.parseContinents(mapText[2]);
        vector<tuple<string, int>> countries = ml.parseCountries(mapText[3]);
        vector<vector<int>> borders = ml.parseBorders(mapText[4]);
        gameMap = new Map(continents, ml.getNumOfContinents(), countries, ml.getNumOfCountries(), borders);
        if (!(*gameMap).validate()) {
            cout << "The map that you've selected has been deemed as invalid. You will be asked to select another one." << endl;
            continue;
        }
        cout << "The selected map has been deemed valid." << endl << endl;

        // all criterias checked
        mapIsValid = true;
    } while (!mapIsValid);
    
    setNbOfPlayers();
    toggleObservers();
    cout << endl;
    for (int i = 0; i < nbOfPlayers; i++) {
        string name;
        cout << "Enter the name for player #" << (i+1) << ": ";
        cin >> name;
        players.push_back(new Player(name));
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

void GameEngine::reinforcementPhase()
{
    for (Player* player : players) {
        int armyValue = 0;
        for (Continent* continent : gameMap->getContinents()) {
            if (ownsContinent(player, continent)) {
                armyValue += continent->getControlValue();
            }
        }
        armyValue += (player->getOwnedCountries().size()) / 3;
        player->setNumOfArmies(armyValue);
    }
}

bool GameEngine::ownsContinent(Player* p, Continent* c)
{
    for (Country* country : c->getCountries()) {
        if (country->getPlayer() != p)
            return false;
    }
    return true;
}

void GameEngine::issueOrdersPhase()
{
    vector<bool> turnIsOver(nbOfPlayers);
    for (bool over : turnIsOver) {
        over = false;
    }

    bool phaseIsOver = true;

    int i = 0;
    int j = 0;
    
    while (true) {
        for (bool over : turnIsOver) {
            if (!over)
                phaseIsOver = false;
        }
        if (phaseIsOver)
            break;
        if (turnIsOver[i])
            continue;

        switch (j) {
        case 0:     // deploy armies to owned countries
            cout << "You currently own the following countries" << endl;
            for (Country* country : players[i]->getOwnedCountries()) {
                cout << "#" << country->getNum() << " " << country->getName() << endl;
            }
            cout << "You currently have " << players[i]->getNumOfArmies() << " armies to deploy." << endl;
            cout << "Enter the number of the country where you want to deploy your armies: ";
            int id;
            cin >> id;
            cout << "Enter the number of armies your want to deploy: ";
            int n;
            cin >> n;



            if (i == nbOfPlayers)
                j++;
            break;
        case 1:     // advance
            for (Country* country : players[i]->toAttack()) {

            }

            if (i == nbOfPlayers)
                j++;
            break;
        case 2:     // special orders
            break;
        }




        phaseIsOver = true;
        
        i = ++i % nbOfPlayers;
    }
}

bool GameEngine::isMapInDirectory(string fileName) {
    ifstream file("maps/" + fileName);
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

void GameEngine::toggleObservers() {
    string answer;
    cout << "Would you like to activate the observers for this game? (Yes or No): ";
    cin >> answer;
    while(!equals(answer,"yes") && !equals(answer,"no")) {
       cout << "Your answer has been deeemd invalid. Please enter again: ";
       cin >> answer;
    }
    if(equals(answer,"yes")) {
        setObserverStatus(true);
        new PhaseObserver(this);
        new GameStatsObserver(this);
    }  
    else if(equals(answer,"no"))
        setObserverStatus(false);
}


// Accessors
int GameEngine::getNbOfPlayers() {
    return nbOfPlayers;
}
Deck* GameEngine::getDeckCards() {
    return deckCards;
}
vector<Player*> GameEngine::getPlayersList() {
    return players;
}
bool GameEngine::getObserverStatus() {
    return activateObservers;
}
void GameEngine::setObserverStatus(bool status) {
    activateObservers = status;
}
Map* GameEngine::getMap() {
    return gameMap;
}
void GameEngine::setPhase(string s){
    this->currentphase = s;
}
string GameEngine::getPhase(){
    return currentphase;
}


// Methods

/* Assigns countries and number of armies to players */
void GameEngine::startupPhase() {
    vector<Country*> randomCountries = (*gameMap).getCountries();
    random_shuffle(randomCountries.begin(), randomCountries.end());     // shuffling the list of countries
    int c = 0;
    for (Country* country : randomCountries) {      // assigning each country to a player
        (*players[c]).setCountry(country);
        country->setPlayer(players[c]);
        c = (c + 1) % nbOfPlayers;
    }

    // assigning initial armies
    int initialArmies = 0;
    switch (nbOfPlayers) {
    case 2:
        initialArmies = 40;
        break;
    case 3:
        initialArmies = 35;
        break;
    case 4:
        initialArmies = 30;
        break;
    case 5:
        initialArmies = 25;
        break;
    }
    for (Player* player : players) {
        player->setNumOfArmies(initialArmies);
    }

}

/* Main flow of the game */
void GameEngine::mainGameLoop(){
    for(int i = 0; i < players.size(); i++){
        // Reinforcement phase
        if(activateObservers){
            setPhase("Player " + players[i]->getName() + ": Reinforcement Phase");
            Notify();
        }

        // Issuing orders phase
        if(activateObservers){
            setPhase("Player " + players[i]->getName() + ": Issuing Orders Phase");
            Notify();
        }

        // Orders execution phase
        if(activateObservers){
            setPhase("Player " + players[i]->getName() + ": Orders Execution Phase");
            Notify();
        }
    }
    
}
