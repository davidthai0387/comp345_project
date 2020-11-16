#include <iterator>
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
};
Orders::Orders(Player* p) {
	exec = false;
	OrderIssuer = p;
};
Orders::Orders(const Orders& o2) {
	name = o2.name;
	exec = o2.exec;
	OrderIssuer = o2.OrderIssuer;
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
void Orders::setOrderIssuer(Player* p){
	OrderIssuer = p;
};
int Orders::getpriority() {
	return priority;
}
Player* Orders::getOrderIssuer(){
	return OrderIssuer;
};

//---------------------DEPLOY CLASS-----------------------
// Constructors
Deploy::Deploy(Player* p, int a, string t) : Orders(p) {
	army = a;
	terr = t;
	this->setName("Deploy");
	priority = 1;
};
Deploy::Deploy(const Deploy& d2) : Orders(d2.OrderIssuer) {
	valid = d2.valid;
	army = d2.army;
	terr = d2.terr;
	this->setName("Deploy");
	priority = 1;
}
Deploy::~Deploy() {
};


// Methods
bool Deploy::validate() {
	if (getArmy() <= this->OrderIssuer->getNumOfArmies() && getArmy() > 0 && this->OrderIssuer->getCountries().find(getTerr()) != string::npos) {
		valid = true;
	}
	return valid;
};
void Deploy::execute() {
	for(Country* c : this->OrderIssuer->getOwnedCountries()){
		if(c->getName() == terr){
			c->setArmies((c->getArmies()) + getArmy());
		}
	}
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
Advance::Advance(Player* p, int a, string t1, string t2) {
	army = a;
	terr1 = t1;
	terr2 = t2;
	this->setName("Advance");
	priority = 4;
};
Advance::Advance(const Advance& a2) {
	valid = a2.valid;
	army = a2.army;
	terr1 = a2.terr1;
	terr2 = a2.terr2;
	this->setName("Advance");
	priority = 4;
}
Advance::~Advance() {
};
// Methods
bool Advance::validate() {
	if (this->OrderIssuer->getCountries().find(getTerr1()) != string::npos && getArmy() <= player's troops in own territory &&  getArmy() > 0 && string terr2 is an adjacent territory) {
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
Bomb::Bomb(Player* p, string t) {
	terr = t;
	this->setName("Bomb");
	priority = 4;
};
Bomb::Bomb(const Bomb& b2) {
	valid = b2.valid;
	terr = b2.terr;
	this->setName("Bomb");
	priority = 4;
}
Bomb::~Bomb() {
};
// Methods
bool Bomb::validate() {
	if ( string terr is an enemy territory ) {
		valid = true;
	}
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
Blockade::Blockade(Player* p, string t) {
	terr = t;
	this->setName("Blockade");
	priority = 3;
};
Blockade::Blockade(const Blockade& bl2) {
	valid = bl2.valid;
	terr = bl2.terr;
	this->setName("Blockade");
	priority = 3;
}
Blockade::~Blockade() {
};
// Methods
bool Blockade::validate() {
	if (this->OrderIssuer->getCountries().find(getTerr()) != string::npos) {
		valid = true;
	}
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
Airlift::Airlift(Player* p, int a, string t1, string t2) {
	army = a;
	terr1 = t1;
	terr2 = t2;
	this->setName("Airlift");
	priority = 2;
};
Airlift::Airlift(const Airlift& ai2) {
	valid = ai2.valid;
	army = ai2.army;
	terr1 = ai2.terr1;
	terr2 = ai2.terr2;
	this->setName("Airlift");
	priority = 2;
}
Airlift::~Airlift() {
};
// Methods
bool Airlift::validate() {
	if (this->OrderIssuer->getCountries().find(getTerr1()) != string::npos && getArmy() <= player's troops in own territory && getArmy() > 0 && string terr2 is a territory) {
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
Negotiate::Negotiate(Player& p) {
	playO = &p;
	this->setName("Negotiate");
	priority = 4;
};
Negotiate::Negotiate(const Negotiate& p2) {
	valid = p2.valid;
	playO = p2.playO;
	playP = p2.playP;
	this->setName("Negotiate");
	priority = 4;
}
Negotiate::~Negotiate() {
};
// Methods
bool Negotiate::validate() {
	if (getPlayerO().getName() != getPlayerP().getName()) {
		valid = true;
	}
	return true;
};
void Negotiate::execute() {
};

void Negotiate::read() {
		cout << "Negotiate\tPrevents attacks to and from " << getPlayerO().getName() << " for one turn" << endl;
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
Player Negotiate::getPlayerO() {
	return *playO;
};
Player Negotiate::getPlayerP() {
	return *playP;
};
string Negotiate::getName() {
	return "Negotiate";
};
void Negotiate::setValid(bool v) {
	valid = v;
};
void Negotiate::setPlayerO(Player *p) {
	playO = p;
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
	vector<Orders*>::iterator ptr;
	for (ptr = list.begin(); ptr < list.end(); ptr++) {
		if (o->getpriority() == 4) {
			this->list.push_back(o);
			break;
		}
		if (o->getpriority() < (*ptr)->getpriority()) {
			this->list.insert(ptr, o);
			break;
		}
	}
	this->list.insert(list.end(), o);
};
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
void OrderList::setList(vector<Orders*> l) {
	this->list = l;
}
string OrderList::displayOrders() {
	string out = "";
	for (int i = 0; i < (signed int)this->getList().size(); i++) {
		if (i == (getList().size() - 1))
			out = out + this->list[i]->getName() + "\n";
		else
			out = out + this->list[i]->getName() + ", ";
	}
	return out;
};
vector<Orders*> OrderList::getList() {
	return this->list;
};
