#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

int main() {
    vector<Player*> listofplayers;
    Player* p1 = new Player("Bob");
    Player* p2 = new Player("Jesus");
    listofplayers.push_back(p1);
    listofplayers.push_back(p2);

    //Checking player name
    cout << p1->getName() << endl;

    //Checking collection of territories
    Country* c1, * c2;
    c1 = new Country(0, "Korea", 0);
    c2 = new Country(1, "Canada", 0);

    p1->setCountry(c1); //Adding countries
    p1->setCountry(c2);
    cout << p1->getCountries() << endl;

    p1->removeCountry("Korea");
    cout << p1->getCountries() << endl;

    p1->setCountry(c1);

    //toAttack method
    cout << "toAttack: " << p1->toAttack() << endl;

    //toDefend method
    cout << "toDefend: " << p1->toDefend() << endl;

    //Cards 
    Deck* deck = new Deck();
    cout << deck->getNumOfCardsInDeck() << endl;;

    deck->draw(p1->getHand());
    cout << p1->getHand().size() << endl;
    cout << p1->showHand() << endl;
    //issueOrder
    p1->issueOrder("Bomb", listofplayers);

    return 0;
}