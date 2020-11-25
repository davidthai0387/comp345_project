#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

class Player;
class Map;
class Deck;
class Hand;
class Card {
private:
    /*The size of the array of the cards that will be given
    at the end of the turn*/
    int cardMinimum;

    //Card name
    string cardName;

public:
    //Default constructor
    Card();
    Card(int cm);
    Card(int cm, string cn);
    Card(const Card& c);

    //Assignment operator
    Card& operator = (const Card& c);

    //Getter
    int getCardMinimum() const;
    string getCardName() const;

    //Setter for CardMinimum 
    void setCardMinimum(int cardMinimum);
    void setCardName(string cardName);

    //Methods
    friend ostream& operator<<(ostream& out, const Card& c);
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i) = 0;
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, int cardNum) = 0;
};

ostream& operator<<(ostream& out, const Card& c);

class BombCard : public Card {
public:
    //Default constructor
    BombCard();
    BombCard(const BombCard& bc);

    //Assignment operator
    BombCard& operator = (const BombCard& bc);

    //Method
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i);
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, int i);
};

class ReinforcementCard : public Card {
public:
    //Default constructor
    ReinforcementCard();
    ReinforcementCard(const ReinforcementCard& rc);

    //Assignment operator
    ReinforcementCard& operator = (const ReinforcementCard& rc);

    //Method
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i);
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, int i);
};

class BlockadeCard : public Card {
public:
    //Default constructor
    BlockadeCard();
    BlockadeCard(const BlockadeCard& blc);

    //Assignment operator
    BlockadeCard& operator = (const BlockadeCard& blc);

    //Method
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i);
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, int i);
};

class AirliftCard : public Card {
public:
    //Default constructor
    AirliftCard();
    AirliftCard(const AirliftCard& ac);

    //Assignment operator
    AirliftCard& operator = (const AirliftCard& ac);

    //Method
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i);
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, int i);
};

class DiplomacyCard : public Card {
public:
    //Default constructor
    DiplomacyCard();
    DiplomacyCard(const DiplomacyCard& dc);

    //Assignment operator
    DiplomacyCard& operator = (const DiplomacyCard& dc);

    //Method
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i);
    virtual void play(Player* p, vector<Player*> o, Map* m, Deck* d, int i);
};

class Deck {
private:
    //The number of repetition you want to create a set of card types (?)
    int cardRep;

    //The actual deck (Can this be a variable of a constructor?)
    vector<Card*> deck;

public:
    //Default constructor
    Deck();
    ~Deck();

    //Parameterized constructor
    Deck(int cardRep, vector<Card*> deck);

    //Copy constructor
    Deck(const Deck& d);

    //Assignment operator
    Deck& operator = (const Deck& d);

    //Setter for cardRep
    void setCardRep(int cardRep);

    //Getter for cardRep
    int getCardRep();

    //Getter for numOfCardsInDeck
    int getNumOfCardsInDeck();

    //Getter for deck
    vector<Card*> getDeck();

    void draw(vector<Card*>& hand);

    void addToDeck(Card* c);

};

vector<Card*> listofcards(int cardRep);


class Hand {

private:
    //The actual hand
    vector<Card*> hand;

public:
    //Default constructor
    Hand();
    Hand(const Hand& h);

    //Assignment operator
    Hand& operator = (const Hand& h);

    //Getter
    vector<Card*>& getHand();

    //Methods
    void discard(int i);

};
