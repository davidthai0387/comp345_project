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

public:
	// Constructor
	Orders();
	Orders(const Orders& o2);
	~Orders();

	// Method
	virtual void read() = 0;
	virtual string getName();
	void setName(string a);
	friend ostream& operator<<(ostream& out, const Orders& o);
};

ostream& operator<<(ostream& out, const Orders& o);

class Deploy : public Orders {
private:
	bool valid{ false };
	int army;
	string terr;

public:
	Deploy(int a, string t);
	Deploy(const Deploy& d2);
	~Deploy();
	bool validate();
	void execute();
	virtual void read();

	bool getValid();
	int getArmy();
	string getTerr();
	string getName();

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

public:
	Advance(int a, string t1, string t2);
	Advance(const Advance& a2);
	~Advance();
	bool validate();
	void execute();
	virtual void read();

	bool getValid();
	int getArmy();
	string getTerr1();
	string getTerr2();
	string getName();

	void setValid(bool v);
	void setArmy(int a);
	void setTerr1(string t);
	void setTerr2(string t);
};

class Bomb : public Orders {
private:
	bool valid{ false };
	string terr;

public:
	Bomb(string t);
	Bomb(const Bomb& b2);
	~Bomb();
	bool validate();
	void execute();
	virtual void read();

	bool getValid();
	string getTerr();
	string getName();

	void setValid(bool v);
	void setTerr(string t);
};

class Blockade : public Orders {
private:
	bool valid{ false };
	string terr;

public:
	Blockade(string terr);
	Blockade(const Blockade& bl2);
	~Blockade();
	bool validate();
	void execute();
	virtual void read();

	bool getValid();
	string getTerr();
	string getName();

	void setValid(bool v);
	void setTerr(string t);
};

class Airlift : public Orders {
private:
	bool valid{ false };
	int army;
	string terr1;
	string terr2;

public:
	//Constructors
	Airlift(int a, string t1, string t2);
	Airlift(const Airlift& ai2);
	~Airlift();

	//Methods
	bool validate();
	void execute();
	virtual void read();

	//Getter
	bool getValid();
	int getArmy();
	string getTerr1();
	string getTerr2();
	string getName();

	//Setter
	void setValid(bool v);
	void setArmy(int a);
	void setTerr1(string t);
	void setTerr2(string t);
};

class Player;
class Negotiate : public Orders {
private:
	bool valid{ false };
	Player* play;

public:
	//Constructor
	Negotiate(Player* p);
	Negotiate(const Negotiate& p2);
	~Negotiate();

	//Methods
	bool validate();
	void execute();
	virtual void read();

	//Getter
	bool getValid();
	Player getPlayer();
	string getName();

	//Setter
	void setValid(bool v);
	void setPlayer(Player* p);
};

class OrderList {
private:
	vector<Orders*> list;

public:
	// Constructors
	OrderList();
	OrderList(const OrderList& l2);
	~OrderList();

	// Methods
	void add(Orders* o);
	void move(int i, int j);
	void remove(int i);
	void execOrders();
	void setList(vector<Orders*> somelist);

	string displayOrders();
	vector<Orders*> getList();
};
