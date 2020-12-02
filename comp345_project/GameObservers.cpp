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
    cout << "\n\n---------------------------------------- " << subject -> getPhase() << "----------------------------------------" << endl;
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
    double Total = subject->getMap()->getCountries().size();
    cout << '|' << setw(10) << "Game Statistics" << setw(8) << '|' << setw(10) << "Owned" << setw(10) << "|" << endl;
    for(int i = 0; i < subject->getPlayersList().size(); i++){
        // Percentage controlled
        double OwnedCountries = subject->getPlayersList()[i]->getOwnedCountries().size();
        double OwnedPercentage = (OwnedCountries/Total)*100;
        cout << '|' << setw(10) << subject->getPlayersList()[i]->getName() << '|' << setw(10) << OwnedPercentage << "%" << '|' << setw(10) << OwnedCountries << setw(10) << '|' << endl;
    }
}