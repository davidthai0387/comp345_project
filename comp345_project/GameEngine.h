#pragma once
#include "Player.h"
#include "GameObservers.h"
#include<iostream>
using namespace std;

class GameEngine
//    : public Observer 
{
    public:
        GameEngine();
        void GameStart();
        int getNbOfPlayers();
        Deck* getDeckCards();
        vector<Player*> getPlayersList();
        bool getObserverStatus();
        void setObserverStatus(bool status);
        Map* getMap();

        // Startup phase methods
        void startupPhase();

        // Main game loop methods
        //void mainGameLoop();


    private:
        int nbOfPlayers;
        Deck* deckCards;
        vector<Player*> players;
        bool activateObservers;
        Map* gameMap;
        bool isMapInDirectory(string fileName);
        bool equals(const string& a, const string& b);
        void setNbOfPlayers();
        bool Observers();
        string selectMap();
};