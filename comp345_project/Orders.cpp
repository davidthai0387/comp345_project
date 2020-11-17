#include <iterator>
#include <algorithm>
#include <random>
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
	orderIssuer = p;
};
Orders::Orders(const Orders& o2) {
	name = o2.name;
	exec = o2.exec;
	orderIssuer = o2.orderIssuer;
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

void Orders::setOrderIssuer(Player* p) {
	orderIssuer = p;
};
int Orders::getpriority() {
	return priority;
}

Player* Orders::getOrderIssuer() {
	return orderIssuer;
};

//---------------------DEPLOY CLASS-----------------------
// Constructors
Deploy::Deploy(Player* p, int a, Country* c, Map* m) : Orders(p) {
	armiesToDeploy = a;
	country = c;
	this->setName("Deploy");
	priority = 1;
};
Deploy::Deploy(const Deploy& d2) : Orders(d2.orderIssuer) {
	valid = d2.valid;
	armiesToDeploy = d2.armiesToDeploy;
	country = d2.country;
	this->setName("Deploy");
	priority = 1;
}
Deploy::~Deploy() {
};


// Methods
bool Deploy::validate() {
	if (getArmy() <= this->orderIssuer->getNumOfArmies() && getArmy() > 0 && this->orderIssuer->getCountryNames().find(getCountry()->getName()) != string::npos) {
		valid = true;
	}
	return valid;
};
void Deploy::execute() {
	for (Country* c : this->orderIssuer->getOwnedCountries()) {
		if (c->getName() == country->getName()) {

			c->setArmies((c->getArmies()) + getArmy());
		}
	}
};
void Deploy::read() {
	cout << "Deploy\t\tPlace " << getArmy() << " troop(s) in " << getCountry()->getName() << endl;
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
	return armiesToDeploy;
};
Country* Deploy::getCountry() {
	return country;
};
string Deploy::getName() {
	return "Deploy";
};
void Deploy::setValid(bool v) {
	valid = v;
};
void Deploy::setArmy(int a) {
	armiesToDeploy = a;
};
void Deploy::setCountry(Country* t) {
	country = t;
};

//---------------------ADVANCE CLASS-----------------------
// Constructors
Advance::Advance(Player* p, int a, Country* c1, Country* c2, Map* m, Deck* d) : Orders(p) {
	armiesToAdvance = a;
	src = c1;
	dest = c2;
	map = m;
	deck = d;
	this->setName("Advance");
	priority = 4;
};
Advance::Advance(const Advance& a2) {
	valid = a2.valid;
	armiesToAdvance = a2.armiesToAdvance;
	src = a2.src;
	dest = a2.dest;
	this->setName("Advance");
	priority = 4;
}
Advance::~Advance() {
};
// Methods
bool Advance::validate() {
	if (/* orderIssuer->getTruce() != dest->getPlayer() && */(*orderIssuer).getCountryNames().find(getSrc()->getName()) != string::npos && getArmiesToDeploy() <= src->getArmies() && getArmiesToDeploy() > 0 && std::find((map->getAllBorders()[src->getNum()]).begin(), (map->getAllBorders()[src->getNum()]).end(), dest) != (map->getAllBorders()[src->getNum()]).end()) {

		valid = true;
	}
	return valid;
};
void Advance::execute() {
	src->setArmies((src->getArmies() - getArmiesToDeploy()));
	for (Country* c : this->orderIssuer->getOwnedCountries()) {
		if (c->getName() == dest->getName()) {
			c->setArmies((c->getArmies()) + getArmiesToDeploy());
			return;
		}
	}
	int attackUnits = getArmiesToDeploy();
	int defendUnits = dest->getArmies();
	while (attackUnits != 0 && defendUnits != 0) {
		int attackPercent = rand() % 101;
		int defendPercent = rand() % 101;
		if (attackPercent >= 40)
			defendUnits--;
		if (defendPercent >= 30)
			attackUnits--;
	}
	if (attackUnits == 0)
		dest->setArmies(defendUnits);
	if (defendUnits == 0) {
		dest->setArmies(attackUnits);
		if (attackUnits != 0) {
			dest->getPlayer()->removeCountry(dest->getName());
			dest->setPlayer(orderIssuer);
			orderIssuer->setCountry(dest);
			// ADD 1 CARD TO orderIssuer HAND
			// LIMIT 1 PER TURN
		}
	}
};
void Advance::read() {
	cout << "Advance\t\tMove " << getArmiesToDeploy() << " troop(s) from " << getSrc()->getName() << " to " << getDest()->getName() << endl;
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
int Advance::getArmiesToDeploy() {
	return armiesToAdvance;
};
Country* Advance::getSrc() {
	return src;
}
Country* Advance::getDest() {
	return dest;
};
string Advance::getName() {
	return "Advance";
};
void Advance::setValid(bool v) {
	valid = v;
};
void Advance::setArmy(int a) {
	armiesToAdvance = a;
};
void Advance::setSrc(Country* c) {
	src = c;
};
void Advance::setDest(Country* c) {
	dest = c;
};

//---------------------BOMB CLASS-----------------------
// Constructors
Bomb::Bomb(Player* p, Country* c, Map* m) : Orders(p) {
	targetCountry = c;
	map = m;
	this->setName("Bomb");
	priority = 4;
};
Bomb::Bomb(const Bomb& b2) {
	valid = b2.valid;
	targetCountry = b2.targetCountry;
	this->setName("Bomb");
	priority = 4;
}
Bomb::~Bomb() {
};
// Methods
bool Bomb::validate() {
	for (Country* country : (*orderIssuer).getOwnedCountries()) {
		if (std::find((map->getAllBorders()[country->getNum()]).begin(), (map->getAllBorders()[country->getNum()]).end(), targetCountry) != (map->getAllBorders()[country->getNum()]).end()) {
			valid = true;
			return true;
		}
	}
	return false;
};
void Bomb::execute() {
	targetCountry->setArmies((targetCountry->getArmies() / 2));
};
void Bomb::read() {
	cout << "Bomb\t\tEliminate half the troops in " << getTargetCountry()->getName() << endl;
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
Country* Bomb::getTargetCountry() {
	return targetCountry;
};
string Bomb::getName() {
	return "Bomb";
};
void Bomb::setValid(bool v) {
	valid = v;
};
void Bomb::setTargetCountry(Country* c) {
	targetCountry = c;
};

//---------------------BLOCKADE CLASS-----------------------
// Constructors
Blockade::Blockade(Player* p, Country* c, Map* m) : Orders(p) {
	target = c;
	map = m;

	this->setName("Blockade");
	priority = 3;
};
Blockade::Blockade(const Blockade& bl2) {
	valid = bl2.valid;
	target = bl2.target;

	this->setName("Blockade");
	priority = 3;
}
Blockade::~Blockade() {
};
// Methods
bool Blockade::validate() {
	if ((*orderIssuer).getCountryNames().find(getTarget()->getName()) != string::npos) {

		valid = true;
	}
	return true;
};
void Blockade::execute() {
	target->setArmies((target->getArmies() * 2));
	orderIssuer->removeCountry(target->getName());
	// GIVE IT TO NEAUTRAL PLAYER
};
void Blockade::read() {
	cout << "Blockade\tTriples troops in " << getTarget()->getName() << " and making it a neutral territory" << endl;
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
Country* Blockade::getTarget() {
	return target;
};
string Blockade::getName() {
	return "Blockade";
};
void Blockade::setValid(bool v) {
	valid = v;
};
void Blockade::setTarget(Country* c) {
	target = c;
};

//---------------------AIRLIFT CLASS-----------------------
// Constructors
Airlift::Airlift(Player* p, int a, Country* c1, Country* c2, Map* m, Deck* d) : Orders(p) {
	armies = a;
	src = c1;
	dest = c2;
	map = m;
	deck = d;
	this->setName("Airlift");
	priority = 2;
};
Airlift::Airlift(const Airlift& ai2) {
	valid = ai2.valid;
	armies = ai2.armies;
	src = ai2.src;
	dest = ai2.dest;
	deck = ai2.deck;
	this->setName("Airlift");
	priority = 2;
}
Airlift::~Airlift() {
};
// Methods
bool Airlift::validate() {
	if (/* orderIssuer->getTruce() != dest->getPlayer() && */(*orderIssuer).getCountryNames().find(getSrc()->getName()) != string::npos && armies <= src->getArmies() && armies > 0) {
		valid = true;
	}
	return valid;
};
void Airlift::execute() {
	src->setArmies((src->getArmies() - armies));
	for (Country* c : this->orderIssuer->getOwnedCountries()) {
		if (c->getName() == dest->getName()) {
			c->setArmies((c->getArmies()) + armies);
			return;
		}
	}
	int attackUnits = armies;
	int defendUnits = dest->getArmies();
	while (attackUnits != 0 && defendUnits != 0) {
		int attackPercent = rand() % 101;
		int defendPercent = rand() % 101;
		if (attackPercent >= 40)
			defendUnits--;
		if (defendPercent >= 30)
			attackUnits--;
	}
	if (attackUnits == 0)
		dest->setArmies(defendUnits);
	if (defendUnits == 0) {
		dest->setArmies(attackUnits);
		if (attackUnits != 0) {
			dest->getPlayer()->removeCountry(dest->getName());
			dest->setPlayer(orderIssuer);
			orderIssuer->setCountry(dest);
			// ADD 1 CARD TO orderIssuer HAND
			// LIMIT 1 PER TURN
		}
	}
};
void Airlift::read() {
	cout << "Airlift\t\tMove " << getArmies() << " troop(s) from " << getSrc()->getName() << " to " << getDest()->getName() << endl;
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
int Airlift::getArmies() {
	return armies;
};
Country* Airlift::getSrc() {
	return src;
};
Country* Airlift::getDest() {
	return dest;
};

string Airlift::getName() {
	return "Airlift";
};
void Airlift::setValid(bool v) {
	valid = v;
};
void Airlift::setArmy(int a) {
	armies = a;
};
void Airlift::setSrc(Country* c) {
	src = c;
};
void Airlift::setDest(Country* c) {
	dest = c;
};

//---------------------NEGOTIATE CLASS-----------------------
Negotiate::Negotiate(Player* p, Player* o, Map* m) : Orders(p) {
	opponent = o;
	map = m;
	this->setName("Negotiate");
	priority = 4;
};
Negotiate::Negotiate(const Negotiate& p2) {
	valid = p2.valid;
	opponent = p2.opponent;
	this->setName("Negotiate");
	priority = 4;
}
Negotiate::~Negotiate() {
};
// Methods
bool Negotiate::validate() {
	if ((*orderIssuer).getName() != (*opponent).getName()) {
		valid = true;
	}
	return true;
};
void Negotiate::execute() {
	// orderIssuer->setTruce(opponent);
	// opponent->setTruce(orderIssuer);
	// AT THE BEGINNING OF EVERY ROUND, SET EVERY PLAYER'S TRUCE TO NOBODY
};

void Negotiate::read() {
	cout << "Negotiate\tPrevents attacks to and from " << (*opponent).getName() << " for one turn" << endl;
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
Player* Negotiate::getOpponent() {
	return opponent;
};
string Negotiate::getName() {
	return "Negotiate";
};
void Negotiate::setValid(bool v) {
	valid = v;
};
void Negotiate::setOpponent(Player* p) {
	opponent = p;
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
