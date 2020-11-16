#include "Player.h"
#include <iostream>
#include <vector>
using namespace std;

// Constructors
Player::Player() :Name("David") {
    this->PlayerHand = new Hand();
    this->PlayerOrders = new OrderList();
};
Player::Player(string a) :Name(a) {
    this->PlayerHand = new Hand();
    this->PlayerOrders = new OrderList();
};
Player::Player(string a, vector<Country*> b, OrderList* c) :Name(a), Countries(b), PlayerOrders(c) {
    this->PlayerHand = new Hand();
    this->PlayerOrders = new OrderList();
};
Player::Player(const Player& p) {
    this->Name = p.Name;
    this->Countries = p.Countries;
    this->PlayerHand = new Hand(*p.PlayerHand);
    this->PlayerOrders = new OrderList(*p.PlayerOrders);
};

Player::~Player() {
    for (Country* country : Countries)
        delete country;

    delete PlayerHand;
    delete PlayerOrders;
};

// Setter methods
void Player::setCountry(Country* a) {
    this->Countries.insert(Countries.end(), a);
};
void Player::setPlayerOrders(Orders* a) {
    this->PlayerOrders->add(a);
}
void Player::setNumOfArmies(int num) {
    numOfArmies = num;
};


// Getter methods
string Player::getName() {
    return this->Name;
};
string Player::getCountries() {
    string out;
    for (int i = 0; i < Countries.size(); i++) {
        if (i == (Countries.size() - 1))
            out += Countries[i]->getName();
        else
            out += Countries[i]->getName() + ", ";
    }
    return out;
};
vector<Country*> Player::getOwnedCountries(){
    return this->Countries;
};
vector<Card*>& Player::getHand() {
    return this->PlayerHand->getHand();
};

string Player::showHand() {
    string out;
    for (int i = 0; i < PlayerHand->getHand().size(); i++) {
        if (i == (PlayerHand->getHand().size() - 1))
            out += PlayerHand->getHand()[i]->getCardName();
        else
            out += PlayerHand->getHand()[i]->getCardName() + ", ";
    }
    return out;
};
OrderList* Player::getPlayerOrders() {
    return this->PlayerOrders;
}
int Player::getNumOfArmies() {
    return numOfArmies;
};

// Deleting methods
void Player::removeCountry(string a) {
    for (int i = 0; i < Countries.size(); i++) {
        if (Countries[i]->getName() == a)
            Countries.erase(Countries.begin() + i);
    };
};
void Player::removeHand(int a) {

};

// Methods
ostream& operator<<(ostream& out, const Player& p) {
    return out << p.Name;
};

Player Player::operator=(const Player& p) {
    this->Name = p.Name;
    this->Countries = p.Countries;
    this->PlayerHand = new Hand(*p.PlayerHand);
    this->PlayerOrders = new OrderList(*p.PlayerOrders);
    return *this;
};
vector<Country*> Player::toDefend() {
    vector<Country*> out;

    // TODO

    return out;
};
vector<Country*> Player::toAttack() {
    vector<Country*> out;

    // TODO

    return out;
};
void Player::issueOrder(string usedcard) {
    issueOrder(usedcard, vector<Player*>());
};
void Player::issueOrder(string usedcard, vector<Player*> listofplayers) {
    //Create new Order pointer
    Orders* o;

    //Issue Order
    if (usedcard == "Bomb") {
        string terr;
        cout << "Where would you like to bomb? ";
        cin >> terr;
        o = new Bomb(terr);

    }
    else if (usedcard == "Reinforcement") {
        int army;
        string terr;
        cout << "How many troops would you like to deploy? ";
        cin >> army;
        cout << "Where would you like to deploy? ";
        cin >> terr;

        o = new Deploy(army, terr);
    }
    else if (usedcard == "Blockade") {
        string terr;
        cout << "Where would you like to form a blockade? ";
        cin >> terr;

        o = new Blockade(terr);
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

        o = new Airlift(army, terr1, terr2);
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

        o = new Negotiate(p);
    }
    else {
        cout << "Invalid card" << endl;
        return;
    }

    //Adding to list of player orders
    this->PlayerOrders->add(o);
    cout << "The order has been issued." << endl;
};
