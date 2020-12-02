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
    friend ostream & operator<<(ostream &out, const MapLoader &ml); 
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

//Adaptee class
class ConquestFileReader {
  public:
        ConquestFileReader();
        ConquestFileReader(string text);
        ConquestFileReader operator=(const ConquestFileReader& cfr);
        ConquestFileReader(const ConquestFileReader& cfr);
        friend ostream & operator<<(ostream &out, const ConquestFileReader &cfr); 
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

//Adapter class
class ConquestFileReaderAdapter : public MapLoader{
    private:
        ConquestFileReader *conquest_;
    public:
        ConquestFileReaderAdapter(ConquestFileReader *conquest) : conquest_(conquest) {}
        ~ConquestFileReaderAdapter();
        virtual vector<string> read() override; 
        virtual bool checkFormat(vector<string> text) override;
        virtual int getNumOfCountries() override;
        virtual int getNumOfContinents() override;
        virtual vector<tuple<string, int>> parseContinents(string text) override;
        virtual vector<tuple<string, int>> parseCountries(string text) override;
        virtual vector<vector<int>> parseBorders(string text) override;
};