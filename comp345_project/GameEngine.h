#pragma once
#include "Player.h"
#include "GameObservers.h"

#include<iostream>

using namespace std;

class GameEngine : public Observable{
    public:
        // Constructors
        GameEngine();
        ~GameEngine();

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
        bool isMapInDirectory(string fileName);
        bool equals(const string& a, const string& b);
        void setNbOfPlayers();
        void toggleObservers();
        string selectMap();
        string currentphase;
        bool ownsContinent(Player* p, Continent* c);
};