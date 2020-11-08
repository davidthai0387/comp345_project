#pragma once
#include "Player.h"
#include<iostream>
using namespace std;

class GameEngine {
    public:
        GameEngine();
        void GameStart();
        int getNbOfPlayers();
        Deck getDeckCards();
        vector<Player> getPlayersList();
        bool getObserverStatus();
        void setObserverStatus(bool status);
        Map getMap();

    private:
        int nbOfPlayers;
        Deck deckCards;
        vector<Player> players;
        bool activateObservers;
        Map gameMap;
        bool isMapInDirectory(string fileName);
        bool equals(const string& a, const string& b);
        void setNbOfPlayers();
        bool Observers();
        string selectMap();
};