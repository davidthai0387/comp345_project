#pragma once
#ifndef MAPLOADER_H
#define MAPLOADER_H

#include<iostream>
using namespace std;

class MapLoader {
public:
    MapLoader(string text);
    MapLoader();
    MapLoader(const MapLoader& ml);
    MapLoader operator=(const MapLoader& ml);
    //~MapLoader();
    vector<string> read();
    int getNumOfCountries();
    int getNumOfContinents();
    bool checkFormat(vector<string> text);
    vector<vector<int>> parseBorders(string text);
    vector<tuple<string, int>> parseContinents(string text);
    vector<tuple<string, int>> parseCountries(string text);

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
#endif
