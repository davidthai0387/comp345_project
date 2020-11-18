#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "Map.h"
using namespace std;

class Player;

class Orders {
private:
	string name;
	
protected:
	bool exec;
	int priority;
	Player* orderIssuer;
	Map* map;
	Deck* deck;

public:
	// Constructor
	Orders();
	Orders(Player* p);
	Orders(const Orders& o2);

	// Destructor
	~Orders();

	// Method
	virtual void read() = 0;
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
	bool valid{ false };
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
	void execute();
	virtual void read();

	// Getters
	bool getValid();
	int getArmy();
	Country* getCountry();
	string getName();

	// Setters
	void setValid(bool v);
	void setArmy(int a);
	void setCountry(Country* t);
};

class Advance : public Orders {
private:
	bool valid{ false };
	int armiesToAdvance;
	Country* src;
	Country* dest;

public:
	// Constructor
	Advance(Player* p, int a, Country* c1, Country* c2, Map* m, Deck* d);

	Advance(const Advance& a2);

	// Destructor
	~Advance();

	// Method
	bool validate();
	void execute();
	virtual void read();

	// Getters
	bool getValid();
	int getArmiesToDeploy();
	Country* getSrc();
	Country* getDest();
	string getName();

	// Setters
	void setValid(bool v);
	void setArmy(int a);
	void setSrc(Country* c);
	void setDest(Country* c);
};

class Bomb : public Orders {
private:
	bool valid{ false };
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
	void execute();
	virtual void read();

	// Getters
	bool getValid();
	Country* getTargetCountry();
	string getName();

	// Setters
	void setValid(bool v);
	void setTargetCountry(Country* c);
};

class Blockade : public Orders {
private:
	bool valid{ false };
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
	void execute();
	virtual void read();

	// Getters
	bool getValid();
	Country* getTarget();
	string getName();

	// Setters
	void setValid(bool v);
	void setTarget(Country* c);
};

class Airlift : public Orders {
private:
	bool valid{ false };
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
	void execute();
	virtual void read();

	//Getters
	bool getValid();
	int getArmies();
	Country* getSrc();
	Country* getDest();
	string getName();

	//Setters
	void setValid(bool v);
	void setArmy(int a);
	void setSrc(Country* c);
	void setDest(Country* c);
};


class Negotiate : public Orders {
private:
	bool valid{ false };
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
	void execute();
	virtual void read();

	//Getters
	bool getValid();
	Player* getOpponent();
	string getName();

	//Setters
	void setValid(bool v);
	void setOpponent(Player* p);
};

class OrderList {

private:
	vector<Orders*> list;

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
	void execOrders();
	string displayOrders();
	void setList(vector<Orders*> somelist);

	// Getters
	vector<Orders*> getList();
};
