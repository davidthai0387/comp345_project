#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class Orders {
private:
	string name;

protected:
	bool exec;
	int priority;

public:
	// Constructor
	Orders();
	Orders(const Orders& o2);

	// Destructor
	~Orders();

	// Method
	virtual void read() = 0;
	virtual string getName();
	void setName(string a);
	int getpriority();
	friend ostream& operator<<(ostream& out, const Orders& o);
};

ostream& operator<<(ostream& out, const Orders& o);

class Deploy : public Orders {
private:
	bool valid{ false };
	int army;
	string terr;
	const Player* play;

public:
	// Constructor
	Deploy(int a, string t, Player& p);
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
	string getTerr();
	string getName();
	Player getPlayer();

	// Setters
	void setValid(bool v);
	void setArmy(int a);
	void setTerr(string t);
};

class Advance : public Orders {
private:
	bool valid{ false };
	int army;
	string terr1;
	string terr2;
	const Player* play;

public:
	// Constructor
	Advance(int a, string t1, string t2, Player& p);
	Advance(const Advance& a2);

	// Destructor
	~Advance();

	// Method
	bool validate();
	void execute();
	virtual void read();

	// Getters
	bool getValid();
	int getArmy();
	string getTerr1();
	string getTerr2();
	string getName();
	Player getPlayer();

	// Setters
	void setValid(bool v);
	void setArmy(int a);
	void setTerr1(string t);
	void setTerr2(string t);
};

class Bomb : public Orders {
private:
	bool valid{ false };
	string terr;
	const Player* play;

public:
	// Constructor
	Bomb(string t, Player& p);
	Bomb(const Bomb& b2);

	// Destructor
	~Bomb();

	// Method
	bool validate();
	void execute();
	virtual void read();

	// Getters
	bool getValid();
	string getTerr();
	string getName();
	Player getPlayer();

	// Setters
	void setValid(bool v);
	void setTerr(string t);
};

class Blockade : public Orders {
private:
	bool valid{ false };
	string terr;
	const Player* play;

public:
	// Constructor
	Blockade(string terr, Player& p);
	Blockade(const Blockade& bl2);

	// Destructor
	~Blockade();

	// Method
	bool validate();
	void execute();
	virtual void read();

	// Getters
	bool getValid();
	string getTerr();
	string getName();
	Player getPlayer();

	// Setters
	void setValid(bool v);
	void setTerr(string t);
};

class Airlift : public Orders {
private:
	bool valid{ false };
	int army;
	string terr1;
	string terr2;
	const Player* play;

public:
	//Constructors
	Airlift(int a, string t1, string t2, Player& p);
	Airlift(const Airlift& ai2);

	// Destructor
	~Airlift();

	//Method
	bool validate();
	void execute();
	virtual void read();

	//Getters
	bool getValid();
	int getArmy();
	string getTerr1();
	string getTerr2();
	string getName();
	Player getPlayer();

	//Setters
	void setValid(bool v);
	void setArmy(int a);
	void setTerr1(string t);
	void setTerr2(string t);
};

class Player;
class Negotiate : public Orders {
private:
	bool valid{ false };
	const Player* playO;
	const Player* playP;

public:
	//Constructor
	Negotiate(Player& p);
	Negotiate(const Negotiate& p2);

	// Destructor
	~Negotiate();

	//Method
	bool validate();
	void execute();
	virtual void read();

	//Getters
	bool getValid();
	Player getPlayerO();
	Player getPlayerP();
	string getName();

	//Setters
	void setValid(bool v);
	void setPlayerO(Player* p);
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
