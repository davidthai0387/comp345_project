#pragma once
#include "MapLoader.h"
#include "Cards.h"
#include "Orders.h"

using namespace std;

class Player {
private:
    // player name
    string name;

    // list of owned countries
    vector<Country*> countries;

    // hand of cards
    Hand* playerHand;

    // list of orders
    OrderList* playerOrders;

    // reinforcement pool
    int numOfArmies;


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
    void setNumOfArmies(int num);

    // removers
    void removeCountry(string a);
    void removeHand(int a);
    //void removeOrders(Orders a);

    // getters
    string getName();
    bool existsCountry(string a);
    string getCountryNames();
    vector<Country*> getOwnedCountries();
    vector<Card*>& getHand();
    OrderList* getPlayerOrders();
    int getNumOfArmies();

    // methods
    string showHand();
    friend ostream& operator<<(ostream& out, const Player& p);
    Player operator=(const Player& p);
    vector<Country*> toDefend(); //return list of country to defend
    vector<Country*> toAttack(); //return list of country to attack
    void issueOrder(Orders* o);
    void issueOrder(string a, vector<Player*> listofplayers); //create Order object + add to list of orders (optional parameter)

};

// Stream operator
ostream& operator<<(ostream& out, const Player& p);