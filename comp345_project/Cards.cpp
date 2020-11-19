#include "Player.h"
#include <time.h>

//=============================================================================
//Default constructor
Card::Card() {

    /*The size of the array of the cards that will be given
    at the end of the turn*/
    cardMinimum = 0;
    cardName = "Card";
}
Card::Card(int cm) {
    cardMinimum = cm;
}
Card::Card(int cm, string cn)
{
    cardMinimum = cm;
    cardName = cn;
}
Card::Card(const Card& c) {
    cardMinimum = c.cardMinimum;
    cardName = c.cardName;
}

//Assignment operator
Card& Card::operator=(const Card& c) {
    cardMinimum = c.cardMinimum;
    cardName = c.cardName;
    return *this;
}
ostream& operator<<(ostream& out, const Card& c) {
    return out << c.cardName;
};

//Getter
int Card::getCardMinimum() const {
    return this->cardMinimum;
}
string Card::getCardName() const {
    return this->cardName;
}

//Setter
void Card::setCardMinimum(int cm) {
    this->cardMinimum = cm;
}
void Card::setCardName(string cn) {
    this->cardName = cn;
}

//============================================================================= 

//Default constructor
BombCard::BombCard() : Card() {
    
}

//Copy constructor
BombCard::BombCard(const BombCard& bc) {
    this->setCardMinimum(bc.getCardMinimum());
    this->setCardName(bc.getCardName());
}

//Assignment operator
BombCard& BombCard::operator=(const BombCard& bc) {
    this->setCardMinimum(bc.getCardMinimum());
    this->setCardName(bc.getCardName());
    return *this;
}


void BombCard::play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i)
{
    d->addToDeck(h->getHand()[i]);

    
    int cNum = rand() % p->toAttack().size();
    p->issueOrder(new Bomb(p, p->toAttack()[cNum], m));

    h->discard(i);
}

//============================================================================= 

//constructor
ReinforcementCard::ReinforcementCard() {
    setCardMinimum(0);
    setCardName("Reinforcement");
}

ReinforcementCard::ReinforcementCard(const ReinforcementCard& rc) {
    this->setCardMinimum(rc.getCardMinimum());
    this->setCardName(rc.getCardName());
}

//Assignment operator
ReinforcementCard& ReinforcementCard::operator=(const ReinforcementCard& rc) {
    this->setCardMinimum(rc.getCardMinimum());
    this->setCardName(rc.getCardName());
    return *this;
}

void ReinforcementCard::play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i)
{
    d->addToDeck(h->getHand()[i]);

    
    int cNum = rand() % p->getOwnedCountries().size();
    p->issueOrder(new Deploy(p, 5, p->getOwnedCountries()[cNum], m));

    h->discard(i);
}

//============================================================================= 

//constructor
BlockadeCard::BlockadeCard() {
    setCardMinimum(0);
    setCardName("Blockade");
}
BlockadeCard::BlockadeCard(const BlockadeCard& blc) {
    this->setCardMinimum(blc.getCardMinimum());
    this->setCardName(blc.getCardName());
}

//Assignment operator
BlockadeCard& BlockadeCard::operator=(const BlockadeCard& blc) {
    this->setCardMinimum(blc.getCardMinimum());
    this->setCardName(blc.getCardName());
    return *this;
}

void BlockadeCard::play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i)
{
    d->addToDeck(h->getHand()[i]);

    
    int cNum = rand() % p->getOwnedCountries().size();
    p->issueOrder(new Blockade(p, p->getOwnedCountries()[cNum], m));

    h->discard(i);
}

//============================================================================= 

//constructor
AirliftCard::AirliftCard() {
    setCardMinimum(0);
    setCardName("Airlift");
}
AirliftCard::AirliftCard(const AirliftCard& ac) {
    this->setCardMinimum(ac.getCardMinimum());
    this->setCardName(ac.getCardName());
}

//Assignment operator
AirliftCard& AirliftCard::operator=(const AirliftCard& ac) {
    this->setCardMinimum(ac.getCardMinimum());
    this->setCardName(ac.getCardName());
    return *this;
}

void AirliftCard::play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i)
{
    d->addToDeck(h->getHand()[i]);

    
    int c1Num = rand() % p->getOwnedCountries().size();
    int c2Num = rand() % m->getCountries().size();

    int a = rand() % p->getOwnedCountries()[c1Num]->getArmies();

    p->issueOrder(new Airlift(p, a, p->getOwnedCountries()[c1Num], m->getCountries()[c2Num], m, d));

    h->discard(i);
}

//============================================================================= 

//constructor
DiplomacyCard::DiplomacyCard() {
    setCardMinimum(0);
    setCardName("Diplomacy");
}
DiplomacyCard::DiplomacyCard(const DiplomacyCard& dc) {
    this->setCardMinimum(dc.getCardMinimum());
    this->setCardName(dc.getCardName());
}

//Assignment operator
DiplomacyCard& DiplomacyCard::operator=(const DiplomacyCard& dc) {
    this->setCardMinimum(dc.getCardMinimum());
    this->setCardName(dc.getCardName());
    return *this;
}

void DiplomacyCard::play(Player* p, vector<Player*> o, Map* m, Deck* d, Hand* h, int i)
{
    string pName = p->getName();
    string oName;
    int oNum;

    do {
        
        oNum = rand() % o.size();
        oName = o[oNum]->getName();
    } while (pName == oName);

    p->issueOrder(new Negotiate(p, o[oNum], m));
}

//============================================================================= 

//Default constructor
Deck::Deck() {
    cardRep = 5;
    //Should createDeck() function be static?
    deck = listofcards(cardRep);
}

//Parameterized constructor
Deck::Deck(int cr, vector<Card*> d) {
    cardRep = cr;
    deck = d;
}

//Copy constructor
Deck::Deck(const Deck& d) {
    cardRep = d.cardRep;
    deck = d.deck;
}

//Assignment operator
Deck& Deck::operator=(const Deck& d) {

    cardRep = d.cardRep;
    deck = d.deck;
    return *this;
}

//Setter for cardRep
void Deck::setCardRep(int cr) {
    cardRep = cr;
}

//Getter for cardRep
int Deck::getCardRep() {
    return this->cardRep;
}

//Getter for numDeck
int Deck::getNumOfCardsInDeck() {
    return this->deck.size();
}

//Getter for deck
vector<Card*> Deck::getDeck() {
    return this->deck;
}

//Free function to randomly draw a card from the deck
void Deck::draw(vector<Card*>& hand) {

    if (deck.empty())
    {
        cout << "You can't draw! The deck is empty!";
    }

    else {
        //Randomly decide the index of the card that will be drawn
        
        int drawnIndex = (rand() % deck.size());
        //Add that card to the arrayList of the hand
        hand.push_back(deck[drawnIndex]);
        //Erase that card from the arrayList of the deck.
        deck.erase(deck.begin() + drawnIndex);

    }

}

void Deck::addToDeck(Card* c)
{
    deck.push_back(c);
}

//Method to create a deck, maybe I should make a constructor from this...
vector<Card*> listofcards(int cardRep) {
    //The deck as an arrayList (vector)
    const int nbbomb = 3;
    const int nbrein = 1;
    const int nbblock = 2;
    const int nbairlift = 2;
    const int nbdiplo = 1;
    std::vector<Card*> deck;
    //Codes to make the deck.
    for (int i = 0; i < cardRep; i++) {

        for (int i = 0; i < nbbomb; i++) {
            Card* bomb = new BombCard();
            deck.push_back(bomb);
        }

        for (int i = 0; i < nbrein; i++) {
            Card* reinforcement = new ReinforcementCard();
            deck.push_back(reinforcement);
        }

        for (int i = 0; i < nbblock; i++) {
            Card* blockade = new BlockadeCard();
            deck.push_back(blockade);
        }

        for (int i = 0; i < nbairlift; i++) {
            Card* airlift = new AirliftCard();
            deck.push_back(airlift);
        }

        for (int i = 0; i < nbdiplo; i++) {
            Card* diplomacy = new DiplomacyCard();
            deck.push_back(diplomacy);
        }
    }
    return deck;
}

//============================================================================= 

//Default constructor
Hand::Hand() {
}
Hand::Hand(const Hand& h) {
    vector<Card*> hand = h.hand;
}

//Assignment operator
Hand& Hand::operator=(const Hand& h) {
    hand = h.hand;
    return *this;
}

//Getter
vector<Card*>& Hand::getHand() {
    return this->hand;
}

void Hand::discard(int i)
{
    hand.erase(hand.begin() + i);
}
