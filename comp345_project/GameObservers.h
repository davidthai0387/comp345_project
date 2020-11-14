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
        string currentphase;
    
    public:
        // Constructor
        Observable();

        // Destructor
        ~Observable();

        // Accessors
        virtual void setPhase(string s);
        string getPhase();

        // Methods
        virtual void Attach(Observer* o);
        virtual void Detach(Observer* o);
        virtual void Notify();
};

class PhaseObserver : public Observer{
    private:
        Observable* subject;
    public:
        // Constructors
        PhaseObserver(Observable s);
        ~PhaseObserver();

        // Method
        virtual void update();
};

class GameStatsObserver : public Observer{
    private:
        Observable* subject;
    public:
        // Constructors
        GameStatsObserver(Observable s);
        ~GameStatsObserver();

        // Method
        virtual void update();
};