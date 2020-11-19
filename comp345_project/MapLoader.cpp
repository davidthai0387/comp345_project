#include "Player.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <iterator>

using namespace std;
/*Constructor w/ parameter. The string parameter represents the name of the map file*/
MapLoader::MapLoader(string text) {
    fileName = text;
    numOfCountries = 0;
    numOfContinents = 0;
}

/*Constructor w/o parameter*/
MapLoader::MapLoader() {
    fileName = "";
    numOfCountries = 0;
    numOfContinents = 0;
}

/*Copy constructor*/
MapLoader::MapLoader(const MapLoader& ml) {
    fileName = ml.fileName;
    numOfCountries = ml.numOfCountries;
    numOfContinents = ml.numOfContinents;
}

/*Assignment operator*/
MapLoader MapLoader::operator=(const MapLoader& ml) {
    this->fileName = ml.fileName;
    this->numOfContinents = ml.numOfContinents;
    this->numOfCountries = ml.numOfCountries;
    return *this;
};

/*Destructor method to delete objects of the MapLoader class*/
//MapLoader::~MapLoader() {
//    delete& fileName; delete& numOfCountries; delete& numOfContinents;
//}

/*read() method will read the .map file and split it into individuals sections.
Each section will be represented by an index in a vector of strings. The section will
be (in order): file header, files, continents, countries and borders.
Return type: Vector of strings*/
vector<string> MapLoader::read() {
    string mapText;
    string text = "";
    vector<string> tokens;
    ifstream MyReadFile("maps/" + fileName);
    while (getline(MyReadFile, mapText)) {
        text.append(mapText + "\n");
    }
    MyReadFile.close();
    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '[')) {
        tokens.push_back(intermediate);
    }
    return tokens;
}

/*get() method that will return the number of countries in a map. Return type: Integer*/
int MapLoader::getNumOfCountries() {
    return numOfCountries;
}

/*get() method that will return the number of continents in a map. Return type: Integer*/
int MapLoader::getNumOfContinents() {
    return numOfContinents;
}

/*This method will determine and return the number of words in a string. Return type: Integer*/
int MapLoader::countWords(string const& str) {
    stringstream stream(str);
    return distance(istream_iterator<string>(stream), istream_iterator<string>());
}

/*This method will determine whether or not the character passed by parameter is a digit.
Return type: boolean*/
bool is_digit(const char value) {
    return std::isdigit(value);
}

/*This method will determine whether or not a .map file is valid. There are multiple criterias
that need to be met for a .map file to be valid. To start, we will searching for key words
in each of the sections of the files. For the file header, we must make sure that it isn't empty.
We will be calling other methods to make sure that the sections all respect their usual format.
Return type: boolean (true: valid, false: non-valid)*/
bool MapLoader::checkFormat(vector<string> text) {
    if (text[0] == "")
        return false;
    if ((text[1].find("files") == string::npos) || (text[2].find("continents") == string::npos) || (text[3].find("countries") == string::npos) || (text[4].find("borders") == string::npos))
        return false;
    if (!checkFiles(text[1]) || !checkContinents(text[2]) || !checkCountries(text[3]) || !checkBorders(text[4]))
        return false;
    return true;
}

/*Checks the [files] section of the file. Looks for key words (pic, map and crd), if those key words are not found
the file will be considered as non valid.*/
bool MapLoader::checkFiles(string text) {
    if ((text.find("pic") != string::npos) && (text.find("map") != string::npos) && (text.find("crd") != string::npos))
        return true;
    return false;
}

/*Checks the [continents] section of the file. This method will determine if the format of the [continents] section
is respected. The format for this section is: ContinentNumber - ContinentName - color. If every line in the section,
follows the format it'll pass the check. If not, the file will be considered non-valid.*/
bool MapLoader::checkContinents(string text) {
    vector<string> tokens;
    vector<string> continents;
    vector<int> controlValue;
    vector<string> colors;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    for (int i = 1; i < tokens.size(); i++)
        tokens[i - 1] = tokens[i];
    for (int i = tokens.size() - 1; i >= 0; i--) {
        if (tokens[i] == "" || tokens[i].at(0) == ';')
            tokens.erase(tokens.begin() + i);
    }
    while (tokens.back().length() <= 1) {
        tokens.pop_back();
    }

    int loopNum, wordCount = 0;
    for (int i = 0; i < tokens.size(); i++) {
        string s = tokens[i];
        string delimiter = " ";
        if (countWords(s) != 3)
            return false;
        size_t pos = 0;
        string token;
        loopNum = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if (loopNum == 0) {
                if (all_of(token.begin(), token.end(), is_digit))
                    return false;
                continents.push_back(token);
            }
            else if (loopNum == 1) {
                try {
                    controlValue.push_back(stoi(token));
                }
                catch (invalid_argument& e) {
                    return false;
                }
                catch (out_of_range& e) {
                    return false;
                }
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        colors.push_back(s);
    }
    if (continents.size() != controlValue.size())
        return false;
    return true;
}

/*This method will parse through the [continents] section of the file and return the data as vector of tuples of strings and integers.
The method is only called if the file has been considered valid.*/
vector<tuple<string, int>> MapLoader::parseContinents(string text) {
    vector<string> tokens;
    vector<string> continents;
    vector<int> controlValue;
    vector<string> colors;
    vector<tuple<string, int>> parsedContinents;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    for (int i = 1; i < tokens.size(); i++)
        tokens[i - 1] = tokens[i];
    for (int i = tokens.size() - 1; i >= 0; i--) {
        if (tokens[i] == "" || tokens[i].at(0) == ';')
            tokens.erase(tokens.begin() + i);
    }

    int loopNum, wordCount = 0;
    for (int i = 0; i < tokens.size(); i++) {
        string s = tokens[i];
        string delimiter = " ";
        size_t pos = 0;
        string token;
        loopNum = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if (loopNum == 0)
                continents.push_back(token);
            else if (loopNum == 1)
                controlValue.push_back(stoi(token));
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        colors.push_back(s);
    }
    for (int i = 0; i < continents.size(); i++) {
        tuple<string, int> tupleContinents = make_tuple(continents[i], controlValue[i]);
        parsedContinents.push_back(tupleContinents);
    }
    numOfContinents = parsedContinents.size();
    return parsedContinents;
}

/*Checks the [countries] section of the file. This method will determine if the format of the [countries] section
is respected. The format for this section is: CountryNumber - CountryName - ContinentNumber - XCoordinate - YCoordinate.
If every line in the section, follows the format it'll pass the check. If not, the file will be considered non-valid.*/
bool MapLoader::checkCountries(string text) {
    vector<string> tokens;
    vector<int> countryNum;
    vector<string> country;
    vector<int> continentsNum;
    vector<int> xCoordinate;
    vector<int> yCoordinate;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    for (int i = 1; i < tokens.size(); i++)
        tokens[i - 1] = tokens[i];
    for (int i = tokens.size() - 1; i >= 0; i--) {
        if (tokens[i] == "" || tokens[i].at(0) == ';')
            tokens.erase(tokens.begin() + i);
    }
    while (tokens.back().length() <= 1) {
        tokens.pop_back();
    }

    int loopNum, wordCount = 0;
    for (int i = 0; i < tokens.size(); i++) {
        string s = tokens[i];
        string delimiter = " ";
        if (countWords(s) != 5)
            return false;
        size_t pos = 0;
        string token;
        loopNum = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if (loopNum == 0) {
                try {
                    countryNum.push_back(stoi(token));
                }
                catch (invalid_argument& e) {
                    return false;
                }
                catch (out_of_range& e) {
                    return false;
                }
            }
            else if (loopNum == 1) {
                if (all_of(token.begin(), token.end(), is_digit))
                    return false;
                country.push_back(token);
            }
            else if (loopNum == 2) {
                try {
                    continentsNum.push_back(stoi(token));
                }
                catch (invalid_argument& e) {
                    return false;
                }
                catch (out_of_range& e) {
                    return false;
                }
            }
            else if (loopNum == 3) {
                try {
                    xCoordinate.push_back(stoi(token));
                }
                catch (invalid_argument& e) {
                    return false;
                }
                catch (out_of_range& e) {
                    return false;
                }
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        try {
            yCoordinate.push_back(stoi(s));
        }
        catch (invalid_argument& e) {
            return false;
        }
        catch (out_of_range& e) {
            return false;
        }
    }
    if (country.size() != continentsNum.size())
        return false;
    return true;
}

/*This method will parse through the [countries] section of the file and return the data as vector of tuples of strings and integers.
The method is only called if the file has been considered valid.*/
vector<tuple<string, int>> MapLoader::parseCountries(string text) {
    vector<string> tokens;
    vector<string> country;
    vector<int> continentsNum;
    vector<tuple<string, int> > parsedCountries;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    for (int i = 1; i < tokens.size(); i++)
        tokens[i - 1] = tokens[i];
    for (int i = tokens.size() - 1; i >= 0; i--) {
        if (tokens[i] == "" || tokens[i].at(0) == ';')
            tokens.erase(tokens.begin() + i);
    }

    int loopNum, wordCount = 0;
    for (int i = 0; i < tokens.size(); i++) {
        string s = tokens[i];
        string delimiter = " ";
        size_t pos = 0;
        string token;
        loopNum = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if (loopNum == 1)
                country.push_back(token);
            else if (loopNum == 2) {
                continentsNum.push_back(stoi(token));
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
    }
    for (int i = 0; i < country.size(); i++) {
        tuple<string, int> tupleCountries = make_tuple(country[i], continentsNum[i]);
        parsedCountries.push_back(tupleCountries);
    }
    numOfCountries = parsedCountries.size();
    return parsedCountries;
}

/*Checks the [borders] section of the file. This method will determine if the format of the [borders] section
is respected. The format for this section is: CountryNumber - AdjCountry1 - AdjCountry2 - ... - AdjCountryN.
AdjCountry means adjacent country. If every line in the section, follows the format it'll pass the check.
If not, the file will be considered non-valid.*/
bool MapLoader::checkBorders(string text) {
    vector<string> tokens;
    string s;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    for (int i = 1; i < tokens.size(); i++)
        tokens[i - 1] = tokens[i];
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].at(0) == ';')
            tokens.erase(tokens.begin() + i);
    }
    tokens.pop_back();

    for (int i = 0; i < tokens.size(); i++) {
        s = tokens[i];
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
        if (!all_of(s.begin(), s.end(), is_digit))
            return false;
    }

    return true;
}

/*This method will parse through the [borders] section of the file and return the data as vector of vector of integers.
The method is only called if the file has been considered valid.*/
vector<vector<int>> MapLoader::parseBorders(string text) {
    vector<string> tokens;
    vector<vector<int>> parsedBorders;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    for (int i = 1; i < tokens.size(); i++)
        tokens[i - 1] = tokens[i];
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].at(0) == ';')
            tokens.erase(tokens.begin() + i);
    }
    tokens.pop_back();
    for (int i = 0; i < tokens.size(); i++) {
        string s = tokens[i];
        string delimiter = " ";
        size_t pos = 0;
        string token;
        vector<int> borders;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            try {
                borders.push_back(stoi(token));
            }
            catch (invalid_argument& e) {

            }
            s.erase(0, pos + delimiter.length());
        }
        try {
            borders.push_back(stoi(s));
        }
        catch (invalid_argument& e) {

        }
        parsedBorders.push_back(borders);
    }
    return parsedBorders;
}
