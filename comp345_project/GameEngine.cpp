#include "GameEngine.h"
#include "Orders.h"

#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>
#include <errno.h>

using namespace std;

GameEngine::GameEngine() {
    nbOfPlayers = 0;
    deck = new Deck();
    activateObservers = true;
    srand(time(NULL));
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
    
    cout << endl << "<<<<<issueOrdersPhase() START" << endl;
    cout << "DEPLOY" << endl;

    for (Player* p : players) {     // deploy random number of armies to random country until no armies left

        // Observer
        if(activateObservers){
            setPhase("Player " + p->getName() + ": Reinforcement Phase");
            Notify();
        }       

        int armiesThisTurn = p->getNumOfArmies();

        while (armiesThisTurn > 0) {
            
            int nArmies = rand() % armiesThisTurn + 1;
            armiesThisTurn -= nArmies;
            int cNum = rand() % p->getOwnedCountries().size();
            p->issueOrder(new Deploy(p, nArmies, p->getOwnedCountries()[cNum], gameMap));
        }
    }

    cout << endl << "ADVANCE" << endl;

    for (Player* p : players) {     // advance

        // Observer
        if(activateObservers){
            setPhase("Player " + p->getName() + ": Issuing Orders Advance Phase");
            Notify();
        }

        if (p->getOwnedCountries().size() == 0)
            continue;

        
        bool keepPlaying = rand() % 3;
        int c1Num; // src country for advance
        int c2Num; // destination country for advance

        while (keepPlaying) {
            
            c1Num = p->getOwnedCountries()[rand() % p->getOwnedCountries().size()]->getNum();

            bool chooseFrom = rand() % 2;
            if (chooseFrom) {
                
                int attackNum = rand() % p->toAttack().size();
                c2Num = p->toAttack()[attackNum]->getNum();
            }
            else {
                
                int defendNum = rand() % p->toDefend().size();
                c2Num = p->toDefend()[defendNum]->getNum();
            }

            int nArmies = 0;
            if (gameMap->getCountries()[c1Num]->getArmies() > 0) {
                
                nArmies = rand() % gameMap->getCountries()[c1Num]->getArmies();
            }
            p->issueOrder(new Advance(p, nArmies, gameMap->getCountries()[c1Num], gameMap->getCountries()[c2Num], gameMap, deck));

            keepPlaying = rand() % 3;
        }
    }

    cout << endl << "CARDS" << endl;

    for (Player* p : players) {     // cards

        // Observer
        if(activateObservers){
            setPhase("Player " + p->getName() + ": Issuing Orders Cards Phase");
            Notify();
        }

        if (p->getOwnedCountries().size() == 0)
            continue;
        int handSize = p->getHand().size();

        while (handSize > 0) {
            
            if (rand() % 2) {
                
                int cardNum = rand() % handSize;
                Hand* h = p->getHandObject();
                p->getHand()[cardNum]->play(p, players, gameMap, deck, h, cardNum);
                handSize--;
            }
        }
    }
    cout << endl << "issueOrdersPhase() END >>>>>" << endl;
}

void GameEngine::executeOrdersPhase()
{
    cout << endl << "<<<<<executeOrdersPhase() START" << endl;
    for (Player* p : players) {

        // Observer
        if(activateObservers){
            setPhase("Player " + p->getName() + ": Orders Execution Phase");
            Notify();
        }
        for (Orders* o : p->getPlayerOrders()->getList()) {
            if (o->execute()) {
                p->getPlayerOrders()->setCountryConquered(true);
            }
        }

        p->getPlayerOrders()->emptyList();
        
        if (p->getPlayerOrders()->getCountryConquered()) {
            deck->draw(p->getHand());
            p->getPlayerOrders()->setCountryConquered(false);
        }
    }
    
    cout << endl << "<<<<<executeOrdersPhase() END" << endl;
}

bool GameEngine::isMapInDirectory(string fileName) {
    ifstream file("maps/" + fileName);
    if(!file)            
        return false;    
    else                 
        return true;
}

void GameEngine::setNbOfPlayers() {
    while(true){
        int count = 0;
        cout << "Select the number of players that will be participating (Must be between 2 and 5): ";
        cin >> count;
        try{
            if(!(count >= 2 && count <=5)) {
                throw exception();
            }
            nbOfPlayers = count;
            cout << endl;
            break;
        } catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "The numbers of players that you've selected has been deemed invalid. Please try again.\n" << endl;
        }
    }
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
Deck* GameEngine::getDeck() {
    return deck;
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
    startupPhase();
    for(int i = 0; i < players.size(); i++){
        // Observer
        if(activateObservers){
            setPhase("Player " + players[i]->getName() + ": Reinforcement Phase");
            Notify();
        }
    }
    
    // loop mechanism
    int remainingPlayers;
    do {
        remainingPlayers = players.size();
        for (Player* p : players) {
            cout << p->getOwnedCountries().size() << endl;
            if (p->getOwnedCountries().size() == 0) {   // count players remaining
                remainingPlayers--;
                continue;
            }
        }
        issueOrdersPhase();

        executeOrdersPhase();
        
        for (Country* c : gameMap->getCountries()) {
            cout << c->getNum() << ": " << c->getArmies() << endl;
        }

    } while (remainingPlayers > 1);
}
