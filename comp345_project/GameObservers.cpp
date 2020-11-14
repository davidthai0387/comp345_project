#include "GameObservers.h"
#include <list>
#include <iostream>
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


// Accessors
void Observable::setPhase(string s){
    this->currentphase = s;
}
string Observable::getPhase(){
    return currentphase;
}


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
PhaseObserver::PhaseObserver(Observable s){
    s.Attach(this);
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
GameStatsObserver::GameStatsObserver(Observable s){
    s.Attach(this);
}
GameStatsObserver::~GameStatsObserver(){
    delete this->subject;
}


// Methods
void GameStatsObserver::update(){
    cout << "something" << endl;
}