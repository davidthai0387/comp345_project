#pragma once

#include<iostream>
#include<vector>

using namespace std;

class MapLoader {
public:
    MapLoader(string text);
    MapLoader();
    MapLoader(const MapLoader& ml);
    MapLoader operator=(const MapLoader& ml);
    //~MapLoader();
    virtual vector<string> read();
    virtual int getNumOfCountries();
    virtual int getNumOfContinents();
    virtual bool checkFormat(vector<string> text);
    virtual vector<vector<int>> parseBorders(string text);
    virtual vector<tuple<string, int>> parseContinents(string text);
    virtual vector<tuple<string, int>> parseCountries(string text);

private:
    string fileName;
    int numOfCountries;
    int numOfContinents;
    bool checkFiles(string text);
    bool checkCountries(string text);
    bool checkContinents(string text);
    bool checkBorders(string text);
    int countWords(string const& str);
};
