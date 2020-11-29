#include "Player.h"
#include "PlayerStrategies.h"

#include <iostream>
#include <vector>
using namespace std;

// Constructors
Player::Player() :name("Neutral") {
    this->playerHand = new Hand();
    this->playerOrders = new OrderList();
};
Player::Player(string a) :name(a) {
    this->playerHand = new Hand();
this->playerOrders = new OrderList();
};
Player::Player(string a, vector<Country*> b, OrderList* c) :name(a), countries(b), playerOrders(c) {
    this->playerHand = new Hand();
    this->playerOrders = new OrderList();
};
Player::Player(const Player& p) {
    this->name = p.name;
    this->countries = p.countries;
    this->playerHand = new Hand(*p.playerHand);
    this->playerOrders = new OrderList(*p.playerOrders);
};

Player::~Player() {
    for (Country* country : countries)
        delete country;

    delete playerHand;
    delete playerOrders;
};

// Setter methods
void Player::setCountry(Country* a) {
    //if(a->getPlayer() != nullptr && a->getPlayer() == this)
        this->countries.insert(countries.end(), a);
};
void Player::setPlayerOrders(Orders* a) {
    this->playerOrders->add(a);
}
void Player::setNumOfArmies(int num) {
    numOfArmies = num;
}

void Player::setStrategy(PlayerStrategy* newStrat) {
    strategy = newStrat;
}

// Getter methods
string Player::getName() {
    return this->name;
}
bool Player::existsCountry(string a)
{
    return false;
}
;
string Player::getCountryNames() {
    string out;
    for (int i = 0; i < countries.size(); i++) {
        if (i == (countries.size() - 1))
            out += countries[i]->getName();
        else
            out += countries[i]->getName() + ", ";
    }
    return out;
};
vector<string> Player::getNegotiatedPlayers() {
    return this->negotiatedPlayers;
};
void Player::resetNegotiatedPlayers() {
    negotiatedPlayers.clear();
}
Hand* Player::getHandObject()
{
    return playerHand;
}
;
vector<Country*> Player::getOwnedCountries() {
    return this->countries;
};
vector<Card*>& Player::getHand() {
    return this->playerHand->getHand();
};

string Player::showHand() {
    string out;
    for (int i = 0; i < playerHand->getHand().size(); i++) {
        if (i == (playerHand->getHand().size() - 1))
            out += playerHand->getHand()[i]->getCardName();
        else
            out += playerHand->getHand()[i]->getCardName() + ", ";
    }
    return out;
};
OrderList* Player::getPlayerOrders() {
    return this->playerOrders;
}
int Player::getNumOfArmies() {
    return numOfArmies;
};

// Deleting methods
void Player::removeCountry(string a) {
    for (int i = 0; i < countries.size(); i++) {
        if (countries[i]->getName() == a)
            countries.erase(countries.begin() + i);
    };
};
void Player::removeHand(int a) {

};

// Methods
ostream& operator<<(ostream& out, const Player& p) {
    return out << p.name;
};

Player Player::operator=(const Player& p) {
    this->name = p.name;
    this->countries = p.countries;
    this->playerHand = new Hand(*p.playerHand);
    this->playerOrders = new OrderList(*p.playerOrders);
    return *this;
};
vector<Country*> Player::toDefend() {
    return countries;
};
vector<Country*> Player::toAttack() {
    vector<Country*> out;

    for (Country* country : countries) {
        for (Country* border : country->getBorders()) {
            if (border->getPlayer()->getName() != this->getName()
                && find(out.begin(), out.end(), border) == out.end()) {
                out.push_back(border);
            }
        }
    }
    return out;
};
void Player::issueOrder(Orders* o) {
    this->getPlayerOrders()->add(o);
}

void Player::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {
    strategy->issueOrder(orderName, p, o, pool, m);
}

void Player::playCard(vector<Player*> o, Deck* d, Map* m) {
    (*strategy).playCard(this, o, d, m);
}
