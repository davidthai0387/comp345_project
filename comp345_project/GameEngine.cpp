#include "GameEngine.h"
#include "MapLoader.h"
#include "Orders.h"

#include <filesystem>
#include <algorithm>
//#include <stdlib.h>   this include makes 'cout' ambiguous
#include <time.h>
#include <iterator>
#include <iostream>

#include <fstream>
#include <vector>
#include <errno.h>
#include <random>

using namespace std;
namespace fs = std::filesystem;
GameEngine::GameEngine() {
    nbOfPlayers = 0;
    deck = new Deck();
    cout << "The deck has been created" << endl;
    activateObservers = true;
    srand(time(NULL));
}

GameEngine::~GameEngine()
{
    for (Player *p : players) {
        delete p;
        p = nullptr;
    }
    delete deck;
    deck = nullptr;
    delete gameMap;
    gameMap = nullptr;
}

GameEngine::GameEngine(const GameEngine& ge) {
    nbOfPlayers = ge.nbOfPlayers;
    deck = ge.deck;
    activateObservers = ge.activateObservers;
    gameMap = ge.gameMap;
    players = ge.players;
    currentphase = ge.currentphase;
}

GameEngine& GameEngine::operator=(const GameEngine& ge) {
    this->nbOfPlayers = ge.nbOfPlayers;
    this->deck = ge.deck;
    this->activateObservers = ge.activateObservers;
    this->gameMap = ge.gameMap;
    this->players = ge.players;
    this->currentphase = ge.currentphase;
    return *this;
}

ostream& operator<<(ostream& os, const GameEngine& ge) {
    os << ge.nbOfPlayers;
    return os;
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

void GameEngine::GameStart()
{
    bool mapIsValid = false;
    string mapType = selectMapType();

    do
    {
        cout << endl << "Here's a list of the available maps" << endl;
        if (equals(mapType, "C"))
        {
            string path = "conquest/";
            int fileCount = 1;
            vector<string> tempList;
            for (const auto & entry : fs::directory_iterator(path)) {
                tempList.push_back(entry.path().string());
            }
            for (string mapChoice : tempList) {
                int pos = mapChoice.find("/");
                mapChoice = mapChoice.substr(pos);
                mapChoice.erase(0,1);
                mapChoice.pop_back(); mapChoice.pop_back(); mapChoice.pop_back(); mapChoice.pop_back();
                if (mapChoice == ".DS_S")
                    continue;
                cout << "Map #" << fileCount << ": " << mapChoice << endl;
                mapList.push_back(mapChoice);
                fileCount++;
            }
            cout << endl;
            string mapName = selectMap(mapList, mapType);
            // checking if map file exists
            if (mapName.compare("") == 0)
            {
                cout << "The map that you've selected does not exist in this current directory. You will be asked to select another one." << endl;
                continue;
            }
            ConquestFileReader *conquest = new ConquestFileReader(mapName);
            ConquestFileReaderAdapter *ml = new ConquestFileReaderAdapter(conquest);
            // checking map file format
            vector<string> mapText = ml->read();
            if (!ml->checkFormat(mapText))
            {
                cout << "The map that you've selected is incorrectly formatted. You will be asked to select another one." << endl;
                continue;
            }
            // checking if map is valid
            vector<tuple<string, int>> continents = ml->parseContinents(mapText[2]);
            vector<tuple<string, int>> countries = ml->parseCountries(mapText[3]);
            vector<vector<int>> borders = ml->parseBorders(mapText[3]);
            gameMap = new Map(continents, ml->getNumOfContinents(), countries, ml->getNumOfCountries(), borders);
            if (!(*gameMap).validate())
            {
                cout << "The map that you've selected has been deemed as invalid. You will be asked to select another one." << endl;
                continue;
            }
            cout << "The selected map has been deemed valid." << endl << endl;

            // all criterias checked
            mapIsValid = true;
        }
        else if (equals(mapType, "D"))
        {
            string path = "maps/";
            int fileCount = 1;
            vector<string> tempList;
            for (const auto & entry : fs::directory_iterator(path)) {
                tempList.push_back(entry.path().string());
            }
            for (string mapChoice : tempList) {
                int pos = mapChoice.find("/");
                mapChoice = mapChoice.substr(pos);
                mapChoice.erase(0,1);
                mapChoice.pop_back(); mapChoice.pop_back(); mapChoice.pop_back(); mapChoice.pop_back();
                if (mapChoice == ".DS_S")
                    continue;
                cout << "Map #" << fileCount << ": " << mapChoice << endl;
                mapList.push_back(mapChoice);
                fileCount++;
            }
            cout << endl;
            string mapName = selectMap(mapList, mapType);
            // checking if map file exists
            if (mapName.compare("") == 0)
            {
                cout << "The map that you've selected does not exist in this current directory. You will be asked to select another one." << endl;
                continue;
            }
            MapLoader *ml = new MapLoader(mapName);
            // checking map file format
            vector<string> mapText = ml->read();
            if (!ml->checkFormat(mapText))
            {
                cout << "The map that you've selected is incorrectly formatted. You will be asked to select another one." << endl;
                continue;
            }

            vector<tuple<string, int>> continents = ml->parseContinents(mapText[2]);
            vector<tuple<string, int>> countries = ml->parseCountries(mapText[3]);
            vector<vector<int>> borders = ml->parseBorders(mapText[4]);
            gameMap = new Map(continents, ml->getNumOfContinents(), countries, ml->getNumOfCountries(), borders);
            if (!(*gameMap).validate())
            {
                cout << "The map that you've selected has been deemed as invalid. You will be asked to select another one." << endl;
                continue;
            }
            cout << "The selected map has been deemed valid." << endl << endl;

            // all criterias checked
            mapIsValid = true;
        }
    } while (!mapIsValid);

    setNbOfPlayers();
    toggleObservers();
    cout << endl;
    for (int i = 0; i < nbOfPlayers; i++)
    {
        string name;
        cout << "Enter the name for player #" << (i + 1) << ": ";
        cin >> name;
        players.push_back(new Player(name));
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(players.begin(), players.end(), g);     // shuffling the list of players
    cout << endl;
}
string GameEngine::selectMapType()
{
    string mapType;
    cout << "Would you like to play with a Conquest or Domination map? (C for Conquest, D for Domination): ";
    cin >> mapType;
    while (!equals(mapType, "C") && !equals(mapType, "D"))
    {
        cout << "Your answer has been deeemd invalid. Please enter again: ";
        cin >> mapType;
    }
    return mapType;
}
bool GameEngine::isMapInDirectory(string fileName, string type)
{
    bool exist;
    if (equals(type, "C"))
    {
        ifstream file("conquest/" + fileName);
        exist = !file;
    }
    else if (equals(type, "D"))
    {
        ifstream file("maps/" + fileName);
        exist = !file;
    }
    if (exist)
        return false;
    else
        return true;
}

string GameEngine::selectMap(vector<string> mapList, string type)
{
    int map;
    cout << "What map would you like to play with ? (Enter the map number): ";
    cin >> map;
    if (isMapInDirectory(mapList[map-1] + ".map", type)) {
        cout << "You've selected the " << mapList[map-1] << " map."<< endl;
        return mapList[map-1] + ".map";
    }
    else
        return "";
}

void GameEngine::reinforcementPhase()
{
    cout << endl << "<<<<<<<<<< reinforcementPhase() START" << endl;

    cout << "----- Armies received by each player: " << endl;

    for (Player* player : players) {
        int armyValue = 0;
        for (Continent* continent : gameMap->getContinents()) {
            if (ownsContinent(player, continent)) {
                armyValue += continent->getControlValue();
            }
        }
        armyValue += (player->getOwnedCountries().size()) / 3;
        player->setNumOfArmies(armyValue);

        cout << player->getName() << ": " << armyValue << endl;

    }

    cout << ">>>>>>>>>> reinforcementPhase() END" << endl;
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
    cout << endl << "<<<<<<<<<< issueOrdersPhase() START" << endl;

    int i = 0;
    bool roundIsOver = false;
    newRound();
    vector<int> phases(players.size(), 0);

    while (!roundIsOver) {

        switch (phases[i]) {    // check
        case 0:
            if (players[i]->getNumOfArmies() > 0) {
                players[i]->issueOrder("Deploy", players[i], players, deck, gameMap);
            }
            else {
                phases[i]++;
            }
            break;
        case 1:
            if (!players[i]->getAdvancePhaseIsOver()) {
                players[i]->issueOrder("Advance", players[i], players, deck, gameMap);
            }
            else {
                continue;
            }
            break;
        case 2:
            if (!players[i]->getCardPhaseIsOver()) {
                players[i]->issueOrder("Card", players[i], players, deck, gameMap);
            }
            else {
                continue;
            }
            break;
        }




        roundIsOver = true;
        for (Player* p : players) {
            if (!p->getAdvancePhaseIsOver() || !p->getCardPhaseIsOver())
                roundIsOver = false;
        }
        i = ++i % players.size();   // i loops from 0 to number of active players
    }


    for (Player* p : players) {     // deploy

        // Observer
        if (activateObservers) {
            setPhase("Player " + p->getName() + ": Reinforcement Phase");
            Notify();
        }

        int armiesThisTurn = p->getNumOfArmies();

        cout << "----- Player " << p->getName() << " deploying armies: " << endl;

        cout << "Starting with " << armiesThisTurn << " armie(s)" << endl;

        while (armiesThisTurn > 0) {
            int nArmies = rand() % armiesThisTurn + 1;
            armiesThisTurn -= nArmies;
            int cNum = rand() % p->getOwnedCountries().size();
            p->issueOrder(new Deploy(p, nArmies, p->getOwnedCountries()[cNum], gameMap));

            cout << "Deployed: " << nArmies << "\tRemaining: " << armiesThisTurn << endl;

        }
        cout << endl;
    }

    cout << "----- Issuing advance orders: " << endl;

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
                
                if (p->toAttack().size() == 0)
                    return;
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

            cout << "Player " << p->getName() << " issuing Advance order from " << gameMap->getCountries()[c1Num]->getName() << " to " << gameMap->getCountries()[c2Num]->getName() << endl << endl;

            keepPlaying = rand() % 3;
        }
    }

    cout << "----- Playing cards: " << endl;

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

    cout << ">>>>>>>>>> issueOrdersPhase() END" << endl;

}

void GameEngine::executeOrdersPhase()
{
    cout << endl << "<<<<<<<<<< executeOrdersPhase() START" << endl;

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

            cout << "Player " << p->getName() << " draws a card." << endl;

            deck->draw(p->getHand());
            p->getPlayerOrders()->setCountryConquered(false);
        }
    }
    
    cout << endl << ">>>>>>>>>> executeOrdersPhase() END" << endl;

}

void GameEngine::newRound() {
    for (Player* p : players) {
        p->setAdvancePhaseIsOver(false);
        p->setCardPhaseIsOver(false);
    }
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
    cout << "Observer status set to: " << status << endl;
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
    cout << endl << "<<<<<<<<<< startUpPhase() START" << endl;
    vector<Country*> randomCountries = (*gameMap).getCountries();
    random_device rd;
    mt19937 g(rd());
    shuffle(randomCountries.begin(), randomCountries.end(), g);     // shuffling the list of countries
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

    cout << "----- Owner of each country: " << endl;
    for (Country* c : gameMap->getCountries()) {
        cout << c->getNum()+1 << ". " << c->getName() << ": " << c->getPlayer()->getName() << endl;
    }
    cout << endl;
	cout << "----- Reinforcement pool of each player: " << endl;
	for (Player* p : players) {
        cout << p->getName() << ": " << p->getNumOfArmies() << endl;
    }
    cout << endl;
    cout << ">>>>>>>>>> startUpPhase() END" << endl;
}

/* Main flow of the game */
void GameEngine::mainGameLoop(){
    cout << endl << "<<<<<<<<<< mainGameLoop() START" << endl;
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

        reinforcementPhase();

        issueOrdersPhase();

        executeOrdersPhase();

        remainingPlayers = players.size();
        for (Player* p : players) {
            if (p->getOwnedCountries().size() == 0) {   // count players remaining
                remainingPlayers--;
                continue;
            }
        }
        if (remainingPlayers == 1) {
            for (Player* p : players) {
                if (p->getNumOfArmies() != 0) {
                    cout << endl << "Player " << p->getName() << " is the winner!!!" << endl;
                    break;
                }
            }
        }

    } while (remainingPlayers > 1);
    cout << ">>>>>>>>>> mainGameLoop() END";
}

int main() {
    GameEngine ge;
    ge.GameStart();
    ge.startupPhase();
    ge.mainGameLoop();
    return 0;
}

