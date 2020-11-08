#pragma once
#include <vector>
#include <string>
#include "Map.h"
#include "Cards.h"
#include "MapLoader.h"
#include "Orders.h"
using namespace std;


class Player {
private:
    // player name
    string Name;

    // list of owned countries
    vector<Country*> Countries;

    // hand of cards
    Hand* PlayerHand;

    // list of orders
    OrderList* PlayerOrders;


public:
    // constructors
    Player();
    Player(string a);
    Player(string a, vector<Country*> b, OrderList* c);
    Player(const Player& p);

    // destructor
    ~Player();

    // setters
    void setCountry(Country* a);
    void setPlayerOrders(Orders* a);

    // removers
    void removeCountry(string a);
    void removeHand(int a);
    //void removeOrders(Orders a);

    // getters
    string getName();
    bool existsCountry(string a);
    string getCountries();
    vector<Card*>& getHand();
    OrderList* getPlayerOrders();

    // methods
    string showHand();
    friend ostream& operator<<(ostream& out, const Player& p);
    Player operator=(const Player& p);
    string toDefend(); //return list of country to defend
    string toAttack(); //return list of country to attack
    inline void issueOrder(string a);
    void issueOrder(string a, vector<Player*> listofplayers); //create Order object + add to list of orders (optional parameter)

};

// Stream operator
ostream& operator<<(ostream& out, const Player& p);