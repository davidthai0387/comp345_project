#pragma once
#include "Player.h"
#include "GameObservers.h"

#include<iostream>

using namespace std;

class GameEngine : public Observable{
    public:
        // Constructors
        GameEngine();
        GameEngine(const GameEngine& ge);
        ~GameEngine();

        //Operators
        GameEngine& operator=(const GameEngine& ge);
        friend ostream& operator<<(ostream& os, const GameEngine& lst);

        // Accessors
        int getNbOfPlayers();
        Deck* getDeck();
        vector<Player*> getPlayersList();
        bool getObserverStatus();
        void setObserverStatus(bool status);
        Map* getMap();
        void setPhase(string s);
        string getPhase();

        // Methods
        void GameStart();
        void startupPhase();
        void mainGameLoop();
        void reinforcementPhase();
        void issueOrdersPhase();
        void executeOrdersPhase();

    private:
        int nbOfPlayers;
        Deck* deck;
        vector<Player*> players;
        bool activateObservers;
        Map* gameMap;
        vector<string> mapList;
        bool isMapInDirectory(string fileName, string type);
        bool equals(const string& a, const string& b);
        void setNbOfPlayers();
        void toggleObservers();
        string selectMap(vector<string> mapList, string type);
        string selectMapType();
        string currentphase;
        bool ownsContinent(Player* p, Continent* c);
};