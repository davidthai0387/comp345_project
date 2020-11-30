#pragma once
#include "Map.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class Player;
class Orders {
private:
	string name;
	
protected:
	bool exec;
	int priority;
	Player* orderIssuer;
	Deck* deck;

public:
	// Constructor
	Orders();
	Orders(Player* p);
	Orders(const Orders& o2);

	// Destructor
	~Orders();

	// Method
	virtual bool execute() = 0;
	virtual string getName();
	void setName(string a);
	void setOrderIssuer(Player* p);
	int getpriority();
	Player* getOrderIssuer();
	friend ostream& operator<<(ostream& out, const Orders& o);
};

ostream& operator<<(ostream& out, const Orders& o);

class Deploy : public Orders {
private:
	int armiesToDeploy;
	Country* country;
	Map* map;

public:
	// Constructor
	Deploy(Player* p, int a, Country* c, Map* m);
	Deploy(const Deploy& d2);

	// Destructor
	~Deploy();

	// Method
	bool validate();
	virtual bool execute();
	void read();

	// Getters
	int getArmy();
	Country* getCountry();
	string getName();

	// Setters
	void setArmy(int a);
	void setCountry(Country* t);
};

class Advance : public Orders {
private:
	int armiesToAdvance;
	Country* src;
	Country* dest;
	Map* map;

public:
	// Constructor
	Advance(Player* p, int a, Country* c1, Country* c2, Map* m, Deck* d);

	Advance(const Advance& a2);

	// Destructor
	~Advance();

	// Method
	bool validate();
	virtual bool execute();
	void read();

	// Getters
	int getArmiesToDeploy();
	Country* getSrc();
	Country* getDest();
	string getName();

	// Setters
	void setArmy(int a);
	void setSrc(Country* c);
	void setDest(Country* c);
};

class Bomb : public Orders {
private:
	Country* targetCountry;
	Map* map;
public:
	// Constructor
	Bomb(Player* p, Country* c, Map* m);

	Bomb(const Bomb& b2);

	// Destructor
	~Bomb();

	// Method
	bool validate();
	virtual bool execute();
	void read();

	// Getters
	Country* getTargetCountry();
	string getName();

	// Setters
	void setTargetCountry(Country* c);
};

class Blockade : public Orders {
private:
	Country* target;
	Map* map;
public:
	// Constructor
	Blockade(Player* p, Country* c, Map* m);
	Blockade(const Blockade& bl2);

	// Destructor
	~Blockade();

	// Method
	bool validate();
	virtual bool execute();
	void read();

	// Getters
	Country* getTarget();
	string getName();

	// Setters
	void setTarget(Country* c);
};

class Airlift : public Orders {
private:
	int armies;
	Country* src;
	Country* dest;
	Map* map;
public:
	//Constructors
	Airlift(Player* p, int a, Country* c1, Country* c2, Map* m, Deck* d);
	Airlift(const Airlift& ai2);

	// Destructor
	~Airlift();

	//Method
	bool validate();
	virtual bool execute();
	void read();

	//Getters
	int getArmies();
	Country* getSrc();
	Country* getDest();
	string getName();

	//Setters
	void setArmy(int a);
	void setSrc(Country* c);
	void setDest(Country* c);
};

class Negotiate : public Orders {
private:
	Player* opponent;
	Map* map;
public:
	//Constructor
	Negotiate(Player* p, Player* o, Map* m);
	Negotiate(const Negotiate& p2);

	// Destructor
	~Negotiate();

	//Method
	bool validate();
	virtual bool execute();
	void read();

	//Getters
	Player* getOpponent();
	string getName();

	//Setters
	void setOpponent(Player* p);
};

class OrderList {

private:
	vector<Orders*> list;
	bool countryConquered;

public:
	// Constructors
	OrderList();
	OrderList(const OrderList& l2);

	// Destructor
	~OrderList();

	// Method
	void add(Orders* o);
	void move(int i, int j);
	void remove(int i);
	string displayOrders();
	void setList(vector<Orders*> somelist);
	void emptyList();
	void setCountryConquered(bool b);

	// Getters
	vector<Orders*> getList();
	bool getCountryConquered();
};
