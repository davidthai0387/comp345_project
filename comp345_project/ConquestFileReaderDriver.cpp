#include <iostream>
#include "ConquestFileReader.cpp"
#include "Map.cpp"
#include "MapLoader.cpp"
using namespace std;

int main() {
    ConquestFileReader *conquest;
    ConquestFileReaderAdapter adapter(conquest);
    Map* game = adapter.loadMap();
    for (int i = 0; i < game->getContinents().size(); i++) {
        cout << (game->getContinents()[i]->getName()) << " " << (game->getContinents()[i]->getControlValue()) << endl;
    }
    cout << endl;
    for (int i = 0; i < game->getCountries().size(); i++) {
        cout <<  game->getCountries()[i]->getName() << " " << game->getCountries()[i]->getContinentNum() << endl;
    }
    cout << endl;
    return 0;
}