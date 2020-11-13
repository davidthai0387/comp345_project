#pragma once
#include <list>
using namespace std;

class Observer{
    protected:
        // Constructor
        Observer();

    public:
        // Destructor
        ~Observer();

        // Methods
        virtual void update() = 0;
};

class Observable{
    private:
        list<Observer*> *observers;
    
    public:
        // Constructor
        Observable();

        // Destructor
        ~Observable();

        // Methods
        virtual void Attach(Observer* o);
        virtual void Detach(Observer* o);
        virtual void Notify();
};

class PhaseObserver : public Observer{
    public:
        PhaseObserver();
        ~PhaseObserver();
};

class GameStatisticsObserver : public Observer{
    public:
        GameStatisticsObserver();
        ~GameStatisticsObserver();
};