#pragma once
#include<iostream>
#include "MapLoader.h"
#include "Map.h"
using namespace std;

//Desired interface
class ConquestFileReader {
  public:
        ConquestFileReader();
        ConquestFileReader(string text);
        Map* loadMap();
        string fileName;
        int numOfCountries;
        int numOfContinents;
        vector<tuple<string, int>> continentsList;
        int getNumOfCountries();
        int getNumOfContinents();
        vector<string> read();
        bool checkFormat(vector<string> text);
        vector<tuple<string, int>> parseContinents(string text);
        vector<tuple<string, int>> parseCountries(string text);
        vector<vector<int>> parseBorders(string text);
        bool checkFiles(string text);
        bool checkCountries(string text);
        bool checkContinents(string text);
        bool checkBorders(string text);
        bool isMapInDirectory(string fileName);
        string selectMap();
};

//Legacy Component
class MapLoader;

//Adapter wrapper
class ConquestFileReaderAdapter : public MapLoader{
    private:
        ConquestFileReader *conquest_;
    public:
        ConquestFileReaderAdapter(ConquestFileReader *conquest) : conquest_(conquest) {}
        Map* loadMap() {
            return conquest_->loadMap();
        }
        vector<string> read() override {
            return conquest_->read();
        }
        int getNumOfCountries() override {
            return conquest_->getNumOfCountries();
        }
        int getNumOfContinents() override {
            return conquest_->getNumOfContinents();
        }
        vector<tuple<string, int>> parseContinents(string text) override {
            return conquest_->parseContinents(text);
        }
        vector<tuple<string, int>> parseCountries(string text) override {
            return conquest_->parseCountries(text);
        }
        vector<vector<int>> parseBorders(string text) override {
            return conquest_->parseBorders(text);
        }
};