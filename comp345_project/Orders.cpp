#include "Player.h"

ostream& operator<<(ostream& out, const Orders& o) {
	if (o.exec == true) {
		cout << "Your order was executed." << endl;
		return out << o.name << endl;
	}
	else
		return out << "Your order was not executed." << endl;

};
//------------------------------ORDERS CLASS------------------------
// Constructor
Orders::Orders() {
	exec = false;
};
Orders::Orders(const Orders& o2) {
	name = o2.name;
	exec = o2.exec;
}
Orders::~Orders() {
};
// Methods
void Orders::read() {
	cout << "Order" << endl;
};
string Orders::getName() {
	return "Order";
};

void Orders::setName(string a) {
	this->name = a;
}

//---------------------DEPLOY CLASS-----------------------
// Constructors
Deploy::Deploy(int a, string t) {
	army = a;
	terr = t;
	this->setName("Deploy");
};
Deploy::Deploy(const Deploy& d2) {
	valid = d2.valid;
	army = d2.army;
	terr = d2.terr;
	this->setName("Deploy");
}
Deploy::~Deploy() {
};
// Methods
bool Deploy::validate() {
	if (getArmy() <= 10 && getArmy() > 0 /*&& .find(getTerr()) != string::npos*/) {
		valid = true;
	}
	return valid;
};
void Deploy::execute() {
};
void Deploy::read() {
	cout << "Deploy\t\tPlace " << getArmy() << " troop(s) in " << getTerr() << endl;
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		execute();
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
};
bool Deploy::getValid() {
	return valid;
};
int Deploy::getArmy() {
	return army;
};
string Deploy::getTerr() {
	return terr;
};
string Deploy::getName() {
	return "Deploy";
};
void Deploy::setValid(bool v) {
	valid = v;
};
void Deploy::setArmy(int a) {
	army = a;
};
void Deploy::setTerr(string t) {
	terr = t;
};

//---------------------ADVANCE CLASS-----------------------
// Constructors
Advance::Advance(int a, string t1, string t2) {
	army = a;
	terr1 = t1;
	terr2 = t2;
	this->setName("Advance");
};
Advance::Advance(const Advance& a2) {
	valid = a2.valid;
	army = a2.army;
	terr1 = a2.terr1;
	terr2 = a2.terr2;
	this->setName("Advance");
}
Advance::~Advance() {
};
// Methods
bool Advance::validate() {
	if (/*.find(getTerr1()) != string::npos && getArmy() <= player's troops in own territory && */ getArmy() > 0 /*&& string terr2 is an adjacent territory*/) {
		valid = true;
	}
	return valid;
};
void Advance::execute() {
};
void Advance::read() {
	cout << "Advance\t\tMove " << getArmy() << " troop(s) from " << getTerr1() << " to " << getTerr2() << endl;
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		execute();
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
};
bool Advance::getValid() {
	return valid;
};
int Advance::getArmy() {
	return army;
};
string Advance::getTerr1() {
	return terr1;
}
string Advance::getTerr2() {
	return terr2;
};
string Advance::getName() {
	return "Advance";
};
void Advance::setValid(bool v) {
	valid = v;
};
void Advance::setArmy(int a) {
	army = a;
};
void Advance::setTerr1(string t) {
	terr1 = t;
};
void Advance::setTerr2(string t) {
	terr2 = t;
};

//---------------------BOMB CLASS-----------------------
// Constructors
Bomb::Bomb(string t) {
	terr = t;
	this->setName("Bomb");
};
Bomb::Bomb(const Bomb& b2) {
	valid = b2.valid;
	terr = b2.terr;
	this->setName("Bomb");
}
Bomb::~Bomb() {
};
// Methods
bool Bomb::validate() {
	//if ( string terr is an adjacent enemy territory ) {
	//	valid = true;
	//}
	return true;
};
void Bomb::execute() {
};
void Bomb::read() {
	cout << "Bomb\t\tEliminate half the troops in " << getTerr() << endl;
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		execute();
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
};
bool Bomb::getValid() {
	return valid;
};
string Bomb::getTerr() {
	return terr;
};
string Bomb::getName() {
	return "Bomb";
};
void Bomb::setValid(bool v) {
	valid = v;
};
void Bomb::setTerr(string t) {
	terr = t;
};

//---------------------BLOCKADE CLASS-----------------------
// Constructors
Blockade::Blockade(string t) {
	terr = t;
	this->setName("Blockade");

};
Blockade::Blockade(const Blockade& bl2) {
	valid = bl2.valid;
	terr = bl2.terr;
	this->setName("Blockade");
}
Blockade::~Blockade() {
};
// Methods
bool Blockade::validate() {
	//if (.find(getTerr()) != string::npos) {
	//	valid = true;
	//}
	return true;
};
void Blockade::execute() {
};
void Blockade::read() {
	cout << "Blockade\tTriples troops in " << getTerr() << " and making it a neutral territory" << endl;
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		execute();
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
};
bool Blockade::getValid() {
	return valid;
};
string Blockade::getTerr() {
	return terr;
};
string Blockade::getName() {
	return "Blockade";
};
void Blockade::setValid(bool v) {
	valid = v;
};
void Blockade::setTerr(string t) {
	terr = t;
};

//---------------------AIRLIFT CLASS-----------------------
// Constructors
Airlift::Airlift(int a, string t1, string t2) {
	army = a;
	terr1 = t1;
	terr2 = t2;
	this->setName("Airlift");
};
Airlift::Airlift(const Airlift& ai2) {
	valid = ai2.valid;
	army = ai2.army;
	terr1 = ai2.terr1;
	terr2 = ai2.terr2;
	this->setName("Airlift");
}
Airlift::~Airlift() {
};
// Methods
bool Airlift::validate() {
	if (/*.find(getTerr1()) != string::npos && getArmy() <= player's troops in own territory && */ getArmy() > 0 /*&& string terr2 is a territory*/) {
		valid = true;
	}
	return valid;
};
void Airlift::execute() {
};
void Airlift::read() {
	cout << "Airlift\t\tMove " << getArmy() << " troop(s) from " << getTerr1() << " to " << getTerr2() << endl;
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		execute();
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
};
bool Airlift::getValid() {
	return valid;
};
int Airlift::getArmy() {
	return army;
};
string Airlift::getTerr1() {
	return terr1;
};
string Airlift::getTerr2() {
	return terr2;
};

string Airlift::getName() {
	return "Airlift";
};
void Airlift::setValid(bool v) {
	valid = v;
};
void Airlift::setArmy(int a) {
	army = a;
};
void Airlift::setTerr1(string t) {
	terr1 = t;
};
void Airlift::setTerr2(string t) {
	terr2 = t;
};

//---------------------NEGOTIATE CLASS-----------------------
Negotiate::Negotiate(Player* p) {
	play = p;
	this->setName("Negotiate");
};
Negotiate::Negotiate(const Negotiate& p2) {
	valid = p2.valid; play = p2.play;
	this->setName("Negotiate");
}
Negotiate::~Negotiate() {
};
// Methods
bool Negotiate::validate() {
	//if (/* Player p is not self && */ getPlayer() == NULL) {
	//	valid = true;
	//}
	return true;
};
void Negotiate::execute() {
};

void Negotiate::read() {
	cout << "Negotiate\tPrevents attacks to and from " << "" << " for one turn" << endl;
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		execute();
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
};


bool Negotiate::getValid() {
	return valid;
};
Player Negotiate::getPlayer() {
	return *play;
};
string Negotiate::getName() {
	return "Negotiate";
};
void Negotiate::setValid(bool v) {
	valid = v;
};
void Negotiate::setPlayer(Player* p) {
	play = p;
};


//---------------------ORDERLIST CLASS-----------------------
// Constructors
OrderList::OrderList() {
};
OrderList::OrderList(const OrderList& l2) {
	list = l2.list;
}
OrderList::~OrderList() {
	list.clear();
	list.shrink_to_fit();
};
// Methods
void OrderList::add(Orders* o) {
	this->list.push_back(o);
};
void OrderList::setList(vector<Orders*> l) {
	this->list = l;
}
void OrderList::move(int i, int j) {
	if (i < (signed int)getList().size() && i >= 0 && j < (signed int)getList().size() && j >= 0) {
		OrderList tempList = OrderList();
		for (int k = 0; k < (signed)getList().size(); k++) {
			if ((k < j || k > i) && j < i) {
				tempList.add(list.at(k));
			}
			else if (k == j && j < i) {
				tempList.add(list.at(i));
			}
			else if (k > j && k <= i && j < i) {
				tempList.add(list.at(k - 1));
			}
			else if (j == i) {
				tempList.add(list.at(k));
			}
			else if ((k > j || k < i) && j > i) {
				tempList.add(list.at(k));
			}
			else if (k == j && j > i) {
				tempList.add(list.at(i));
			}
			else if (k < j && k >= i && j > i) {
				tempList.add(list.at(k + 1));
			}
		}
		setList(tempList.list);
	}
};
void OrderList::remove(int i) {
	if (i < (signed int)getList().size() && i >= 0) {
		this->list.erase(list.begin() + i);
	}
}
void OrderList::execOrders() {
	for (int i = 0; i < (signed int)getList().size(); i++) {
		list[i]->read();
	}
	while (!getList().empty()) {
		this->list.pop_back();
	}
};
string OrderList::displayOrders() {
	string out = "";
	for (int i = 0; i < this->getList().size(); i++) {
		if (i == (getList().size() - 1)) {
			out = out + this->list[i]->getName() + "\n";
		}
		else
			out = out + this->list[i]->getName() + ", ";
	}
	return out;
};
vector<Orders*> OrderList::getList() {
	return this->list;
};
