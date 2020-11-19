#include "GameEngine.h"

#include <list>
#include <iostream>
#include <iomanip> 
using namespace std;

// ----------------------- Observer -----------------------
// Constructors
Observer::Observer(){
};
Observer::~Observer(){
};


// ----------------------- Observable -----------------------
// Constructors
Observable::Observable(){
    observers = new list<Observer*>;
};
Observable::~Observable(){
    delete observers;
};


// Methods
void Observable::Attach(Observer* o){
    observers -> push_back(o);
};
void Observable::Detach(Observer* o){
    observers -> remove(o);
};
void Observable::Notify(){
    list<Observer*>::iterator i = observers -> begin();
    for (; i != observers->end(); ++i)
        (*i)->update();
};


// ----------------------- PhaseObserver -----------------------
// Constructors
PhaseObserver::PhaseObserver(GameEngine* s){
    s->Attach(this);
    this->subject = s;
}
PhaseObserver::~PhaseObserver(){
    delete this->subject;
}


// Methods
void PhaseObserver::update(){
    cout << "---------------------------------------- " << subject -> getPhase() << "----------------------------------------" << endl;
}


// ----------------------- GameStatsObserver -----------------------
// Constructors
GameStatsObserver::GameStatsObserver(GameEngine* s){
    s->Attach(this);
    this->subject = s;
}
GameStatsObserver::~GameStatsObserver(){
    delete this->subject;
}


// Methods
void GameStatsObserver::update(){
    int Total = subject->getMap()->getCountries().size();
    cout << '|' << setw(20) << "Game Statistics" << setw(20) << '|' << endl;
    for(int i = 0; i < subject->getPlayersList().size(); i++){
        // Percentage controlled
        int OwnedCountries = subject->getPlayersList()[i]->getOwnedCountries().size();
        cout << '|' << setw(10) << subject->getPlayersList()[i]->getName() << '|' << setw(10) << (OwnedCountries/Total)*100 << "%" << '|' << endl;

    }
}