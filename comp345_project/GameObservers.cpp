#include "GameObservers.h"
#include <list>
#include <iostream>
using namespace std;

Observer::Observer(){
};

Observer::~Observer(){
};

Observable::Observable(){
    observers = new list<Observer*>;
};

Observable::~Observable(){
    delete observers;
};

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

PhaseObserver::PhaseObserver(Observable s){
    s.Attach(this);
}

PhaseObserver::~PhaseObserver(){
    delete this->subject;
}

void PhaseObserver::update(){
    cout << "something" << endl;
}

GameStatsObserver::GameStatsObserver(Observable s){
    s.Attach(this);
}

GameStatsObserver::~GameStatsObserver(){
    delete this->subject;
}

void GameStatsObserver::update(){
    cout << "something" << endl;
}