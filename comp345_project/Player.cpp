#include "Player.h"
#include <iostream>
#include <vector>
using namespace std;

// Constructors
Player::Player() :name("David") {
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
    this->countries.insert(countries.end(), a);
};
void Player::setPlayerOrders(Orders* a) {
    this->playerOrders->add(a);
}
void Player::setNumOfArmies(int num) {
    numOfArmies = num;
};


// Getter methods
string Player::getName() {
    return this->name;
};
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
};
void Player::issueOrder(string usedcard, vector<Player*> listofplayers) {
    //Create new Order pointer
    Orders* o;

    //Issue Order
    if (usedcard == "Bomb") {
        string terr;
        cout << "Where would you like to bomb? ";
        cin >> terr;
        //o = new Bomb();

    }
    else if (usedcard == "Reinforcement") {
        int army;
        string terr;
        cout << "How many troops would you like to deploy? ";
        cin >> army;
        cout << "Where would you like to deploy? ";
        cin >> terr;

        //o = new Deploy(army, terr);
    }
    else if (usedcard == "Blockade") {
        string terr;
        cout << "Where would you like to form a blockade? ";
        cin >> terr;

        //o = new Blockade(terr);
    }
    else if (usedcard == "Airlift") {
        int army;
        string terr1;
        string terr2;
        cout << "From which of your territories would like you deploy from? ";
        cin >> terr1;
        cout << "How many troops would you like to deploy? ";
        cin >> army;
        cout << "Which territory would you like to deploy them to? ";
        cin >> terr2;

        //o = new Airlift(army, terr1, terr2);
    }
    else if (usedcard == "Diplomacy") {
        string playername;
        cout << "Which player would you like to negociate with? ";
        cin >> playername;

        //1.Need to obtain list of all players
        //2.Find right player name;
        //3.Add to negotiate
        Player* p = new Player();
        for (int i = 0; i < listofplayers.size(); i++) {
            if (listofplayers[i]->getName() == playername)
                p = listofplayers[i];
        }

        //o = new Negotiate(p);
    }
    else {
        cout << "Invalid card" << endl;
        return;
    }

    //Adding to list of player orders
    //this->playerOrders->add(o);
    cout << "The order has been issued." << endl;
};
