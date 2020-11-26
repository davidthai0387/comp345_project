#pragma once
#include<iostream>
#include "MapLoader.h"
using namespace std;

//Desired interface
class ConquestFileReader {
  public:
        ConquestFileReader();
        ConquestFileReader(string text);
        int getNumOfCountries();
        int getNumOfContinents();
        vector<string> read();
        bool checkFormat(vector<string> text);
        vector<tuple<string, int>> parseContinents(string text);
        vector<tuple<string, int>> parseCountries(string text);
        vector<vector<int>> parseBorders(string text);
    private:
        int numOfCountries;
        int numOfContinents;
        string fileName;
        vector<tuple<string, int>> continentsList;
        bool checkFiles(string text);
        bool checkCountries(string text);
        bool checkContinents(string text);
        bool checkBorders(string text);
};

//Legacy Component
class MapLoader;

//Adapter wrapper
class ConquestFileReaderAdapter : public MapLoader{
    private:
        ConquestFileReader *conquest_;
    public:
        ConquestFileReaderAdapter(ConquestFileReader *conquest) : conquest_(conquest) {}
        virtual vector<string> read() override {
            return this->conquest_->read();
        }
        virtual bool checkFormat(vector<string> text) override {
            return this->conquest_->checkFormat(text);
        }
        virtual int getNumOfCountries() override {
            return this->conquest_->getNumOfCountries();
        }
        virtual int getNumOfContinents() override {
            return this->conquest_->getNumOfContinents();
        }
        virtual vector<tuple<string, int>> parseContinents(string text) override {
            return this->conquest_->parseContinents(text);
        }
        virtual vector<tuple<string, int>> parseCountries(string text) override {
            return this->conquest_->parseCountries(text);
        }
        virtual vector<vector<int>> parseBorders(string text) override {
            return this->conquest_->parseBorders(text);
        }
};