#pragma once
#include <limits.h> 
#include <stdio.h> 
#include "Player.h"

using namespace std;

class PlayerStrategy {
public:
	virtual void issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) = 0;
	virtual vector<Country*> toAttack(Player* p) = 0;
	virtual vector<Country*> toDefend(Player* p) = 0;
	virtual void playCard(Player* p, vector<Player*> o, Deck* d, Map* m) = 0;	// choose a card from hand and calls card.play[cardNum]
};

class HumanPlayer : public PlayerStrategy {
public:

	void issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m);
	vector<Country*> toAttack(Player* p);
	vector<Country*> toDefend(Player* p);
	void playCard(Player* p, vector<Player*> o, Deck* d, Map* m);
};

class AggressiveComputer : public PlayerStrategy {
public:
	void issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m);
	vector<Country*> toAttack(Player* p);
	vector<Country*> toDefend(Player* p);
	void playCard(Player* p, vector<Player*> o, Deck* d, Map* m);
};

class BenevolentComputer : public PlayerStrategy {
public:
	void issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m);
	vector<Country*> toAttack(Player* p);
	vector<Country*> toDefend(Player* p);
	void playCard(Player* p, vector<Player*> o, Deck* d, Map* m);
};

class NeutralComputer : public PlayerStrategy {
public:
	void issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m);
	vector<Country*> toAttack(Player* p);
	vector<Country*> toDefend(Player* p);
	void playCard(Player* p, vector<Player*> o, Deck* d, Map* m);
};