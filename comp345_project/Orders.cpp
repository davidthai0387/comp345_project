#include "Player.h"

#include <iterator>
#include <algorithm>
#include <random>
#include <time.h>
#include <stdlib.h>

using namespace std;

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
		return true;
	}
};
bool Deploy::execute() {
	read();
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		for (Country* c : this->orderIssuer->getOwnedCountries()) {
			if (c->getName() == country->getName()) {
				c->setArmies((c->getArmies()) + getArmy());
			}
		}
	} else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
	return false;
};
void Deploy::read() {
	cout << "Deploy\t\tPlace " << getArmy() << " troop(s) in " << getCountry()->getName() << endl;
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
	string opponent = dest->getPlayer()->getName();
	vector<string> negotiatedPlayers = orderIssuer->getNegotiatedPlayers();
	vector<Country*> borders = map->getAllBorders()[src->getNum()];
	if (count(negotiatedPlayers.begin(), negotiatedPlayers.end(), opponent) == 0 // check if negotiated
		&& (*orderIssuer).getCountryNames().find(getSrc()->getName()) != string::npos // check if issuer owns source
		&& getArmiesToDeploy() <= src->getArmies() // check if armies <= source armies
		&& getArmiesToDeploy() > 0 // check if armies > 0
		) {
		if (std::find(borders.begin(), borders.end(), dest) != (borders.end())) {
			{
				return true;
			}
		}
	}
	return false;
};
bool Advance::execute() {
	read();
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		src->setArmies((src->getArmies() - getArmiesToDeploy()));
		for (Country* c : this->orderIssuer->getOwnedCountries()) {
			if (c->getName() == dest->getName()) {
				c->setArmies((c->getArmies()) + getArmiesToDeploy());
				return false;
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
				return true;
			}
		}
		return false;
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
		return false;
	}
};
void Advance::read() {
	cout << "Advance\t\tMove " << getArmiesToDeploy() << " troop(s) from " << getSrc()->getName() << " to " << getDest()->getName() << endl;
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
	targetCountry = b2.targetCountry;
	this->setName("Bomb");
	priority = 4;
}
Bomb::~Bomb() {
};
// Methods
bool Bomb::validate() {
	string opponent = targetCountry->getPlayer()->getName();
	vector<string> negotiatedPlayers = orderIssuer->getNegotiatedPlayers();
	for (Country* country : (*orderIssuer).getOwnedCountries()) {
		if (count(negotiatedPlayers.begin(), negotiatedPlayers.end(), opponent) == 0 && std::find((map->getAllBorders()[country->getNum()]).begin(), (map->getAllBorders()[country->getNum()]).end(), targetCountry) != (map->getAllBorders()[country->getNum()]).end()) {
			return true;
		}
	}
	return false;
};
bool Bomb::execute() {
	read();
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		targetCountry->setArmies((targetCountry->getArmies() / 2));
	} else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
	return false;
};
void Bomb::read() {
	cout << "Bomb\t\tEliminate half the troops in " << getTargetCountry()->getName() << endl;
};
Country* Bomb::getTargetCountry() {
	return targetCountry;
};
string Bomb::getName() {
	return "Bomb";
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
	target = bl2.target;

	this->setName("Blockade");
	priority = 3;
}
Blockade::~Blockade() {
};
// Methods
bool Blockade::validate() {
    if ((*orderIssuer).getCountryNames().find(getTarget()->getName()) != string::npos) {
		return true;
    }
    return false;
};
bool Blockade::execute() {
	read();
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		target->setArmies((target->getArmies() * 2));
		orderIssuer->removeCountry(target->getName());
		target->setPlayer(new Player);
	}
	else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
	return false;
};
void Blockade::read() {
	cout << "Blockade\tDoubles troops in " << getTarget()->getName() << " and making it a neutral territory" << endl;
	
};
Country* Blockade::getTarget() {
	return target;
};
string Blockade::getName() {
	return "Blockade";
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
	string opponent = dest->getPlayer()->getName();
	vector<string> negotiatedPlayers = orderIssuer->getNegotiatedPlayers();
	if (count(negotiatedPlayers.begin(), negotiatedPlayers.end(), opponent) == 0 && (*orderIssuer).getCountryNames().find(getSrc()->getName()) != string::npos && armies <= src->getArmies() && armies > 0) {
		return true;
	}
	return false;
};
bool Airlift::execute() {
	read();
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		src->setArmies((src->getArmies() - armies));
		for (Country* c : this->orderIssuer->getOwnedCountries()) {
			if (c->getName() == dest->getName()) {
				c->setArmies((c->getArmies()) + armies);
				return false;
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
				return true;
			}
		}
		return false;
	} else {
		cout << "Order is invalid, no action will occur.\n" << endl;
		return false;
	}
	
};
void Airlift::read() {
	cout << "Airlift\t\tMove " << getArmies() << " troop(s) from " << getSrc()->getName() << " to " << getDest()->getName() << endl;
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
	opponent = p2.opponent;
	this->setName("Negotiate");
	priority = 4;
}
Negotiate::~Negotiate() {
};
// Methods
bool Negotiate::validate() {
	if ((*orderIssuer).getName() != (*opponent).getName()) {
		return true;
	}
	return false;
};
bool Negotiate::execute() {
	read();
	if (validate()) {
		cout << "Order is valid, executing...\n" << endl;
		orderIssuer->getNegotiatedPlayers().push_back((*opponent).getName());
		opponent->getNegotiatedPlayers().push_back((*orderIssuer).getName());
	} else {
		cout << "Order is invalid, no action will occur.\n" << endl;
	}
	return false;
};

void Negotiate::read() {
	cout << "Negotiate\tPrevents attacks to and from " << (*opponent).getName() << " for one turn" << endl;
};


Player* Negotiate::getOpponent() {
	return opponent;
};
string Negotiate::getName() {
	return "Negotiate";
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
void OrderList::setList(vector<Orders*> l) {
	this->list = l;
}
void OrderList::emptyList()
{
	while (!list.empty()) {
		list.pop_back();
	}
}
void OrderList::setCountryConquered(bool b)
{
	countryConquered = b;
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
	return list;
}
bool OrderList::getCountryConquered()
{
	return countryConquered;
}
;
