#include "GameEngine.h"

#include <iostream>
#include <time.h>

using namespace std;

int main() {
    GameEngine ge;
    ge.GameStart();
    ge.startupPhase();
    ge.mainGameLoop();
    return 0;
}