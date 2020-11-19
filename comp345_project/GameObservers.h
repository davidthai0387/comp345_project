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

class GameEngine;
class PhaseObserver : public Observer{
    private:
        GameEngine* subject;
    public:
        // Constructors
        PhaseObserver(GameEngine* s);
        ~PhaseObserver();

        // Method
        virtual void update();
};

class GameStatsObserver : public Observer{
    private:
        GameEngine* subject;
        
    public:
        // Constructors
        GameStatsObserver(GameEngine* s);
        ~GameStatsObserver();

        // Method
        virtual void update();
};