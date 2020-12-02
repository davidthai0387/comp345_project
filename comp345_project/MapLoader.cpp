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

/*Stream insertion operator for ConquestFileReader*/
ostream& operator<<(ostream &os, const MapLoader& ml) {
    return os;
}

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
the file will be considered as non valid. Return type: boolean (true: valid, false: non-valid)*/
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
/*Constructor w/o parameter*/
ConquestFileReader::ConquestFileReader() {
    fileName = "";
    numOfContinents = 0;
    numOfCountries = 0;    
}

/*Constructor w/ parameter. The string parameter represents the name of the map file*/
ConquestFileReader::ConquestFileReader(string text) {
    fileName = text;
    numOfContinents = 0;
    numOfCountries = 0;    
}

/*Assignment constructor*/
ConquestFileReader ConquestFileReader::operator=(const ConquestFileReader& cfr) {
    this->fileName = cfr.fileName;
    this->numOfContinents = cfr.numOfContinents;
    this->numOfCountries = cfr.numOfCountries;
    return *this;
}

/*Copy constructor*/
ConquestFileReader::ConquestFileReader(const ConquestFileReader& cfr){
    fileName = cfr.fileName;
    numOfContinents = cfr.numOfContinents;
    numOfCountries = cfr.numOfCountries;
}

/*Stream insertion operator for ConquestFileReader*/
ostream& operator<<(ostream &os, const ConquestFileReader& cfr) {
    return os;
}

/*Destructor*/
ConquestFileReaderAdapter::~ConquestFileReaderAdapter() {
    delete conquest_;
    conquest_ = nullptr;
}

/*countDigits(string str) will return the number of digits in a string
Return type: Integer*/
int countDigits(string str){
  int count=0;
  for(int i=0;i<str.size();i++)
     if(isdigit(str[i])) 
        count++;
  return count;
}

/*get() method that will return the number of territories in a map. Return type: Integer*/
int ConquestFileReader::getNumOfCountries() {
    return numOfCountries;
}

/*get() method that will return the number of continents in a map. Return type: Integer*/
int ConquestFileReader::getNumOfContinents() {
    return numOfContinents;
}

/*read() method will read the .map file and split it into individuals sections.
Each section will be represented by an index in a vector of strings. The section will
be (in order): empty space, map, continents and territories.
Return type: Vector of strings*/
vector<string> ConquestFileReader::read() {
    string mapText;
    string text = "";
    vector<string> tokens;
    ifstream MyReadFile("conquest/" + fileName);
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

/*Checks the [Map] section of the file. Looks for key words (image, wrap, scroll, author and warn), if those key words are not found
the file will be considered as non valid. Return type: boolean (true: valid, false: non-valid)*/
bool ConquestFileReader::checkFiles(string text) {
    if ((text.find("image") == string::npos) || (text.find("wrap") == string::npos) || (text.find("scroll") == string::npos) 
            || (text.find("author") == string::npos) || (text.find("warn") == string::npos))
        return false;
    return true;
}

/*This method will determine whether or not a .map file is valid. There are multiple criterias
that need to be met for a .map file to be valid. To start, we will searching for key words
in each of the sections of the files. For the file header, we must make sure that it is empty.
We will be calling other methods to make sure that the sections all respect their usual format.
Return type: boolean (true: valid, false: non-valid)*/
bool ConquestFileReader::checkFormat(vector<string> text) {
    if (text[0] != "")
        return false;
    if ((text[2].find("Continents") == string::npos) || (text[3].find("Territories") == string::npos))
        return false;
    if (!checkFiles(text[1]) || !checkContinents(text[2]) || !checkCountries(text[3]))
        return false;
    return true;
}

/*Checks the [continents] section of the file. This method will determine if the format of the [continents] section
is respected. The format for this section is: ContinentName=score. If every line in the section,
follows the format it'll pass the check. If not, the file will be considered non-valid.*/
bool ConquestFileReader::checkContinents(string text) {
    vector<string> tokens;
    vector<string> continents;
    vector<int> score;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    tokens.erase(tokens.begin());
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
        string delimiter = "=";
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
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        try {
            score.push_back(stoi(s));
        }
        catch (...) {
            return false;
        }
    }
    if (continents.size() != score.size())
        return false;
    return true;
}

/*This method will parse through the [continents] section of the file and return the data as vector of tuples of strings and integers.
The method is only called if the file has been considered valid.*/
vector<tuple<string, int>> ConquestFileReader::parseContinents(string text) {
    vector<string> tokens;
    vector<string> continents;
    vector<tuple<string, int>> parsedContinents;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    tokens.erase(tokens.begin());
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
        string delimiter = "=";
        size_t pos = 0;
        string token;
        loopNum = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if (loopNum == 0) {
                continents.push_back(token);
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
    }
    continents.pop_back();
    int index = 1;
    for (int i = 0; i < continents.size(); i++) {
        tuple<string, int> tupleContinents = make_tuple(continents[i], index);
        index++;
        parsedContinents.push_back(tupleContinents);
    }
    numOfContinents = parsedContinents.size();
    continentsList = parsedContinents;
    return parsedContinents;
}
/*Checks the [territories] section of the file. This method will determine if the format of the [countries] section
is respected. The format for this section is: TerritoryName - XCoordinate - YCoordinate - ContinentName - Borders...
If every line in the section, follows the format it'll pass the check. If not, the file will be considered non-valid.*/
bool ConquestFileReader::checkCountries(string text) {
    vector<string> tokens;
    vector<string> territoryName;
    vector<string> borders;
    vector<string> continentName;
    vector<int> xCoordinate;
    vector<int> yCoordinate;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    tokens.erase(tokens.begin());
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
        string delimiter = ",";
        size_t pos = 0;
        string token;
        loopNum = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if (loopNum == 0) {
                if (all_of(token.begin(), token.end(), is_digit) || !is_digit(token[token.size() - 1]))
                    return false;
                territoryName.push_back(token);
            }
            else if (loopNum == 1) {
                try {
                    xCoordinate.push_back(stoi(token));
                }
                catch (...) {
                    return false;
                }
            }
            else if (loopNum == 2) {
                try {
                    yCoordinate.push_back(stoi(token));
                }
                catch (...) {
                    return false;
                }
            }
            else if (loopNum == 3) 
                continentName.push_back(token);
            else {
                if (all_of(token.begin(), token.end(), is_digit) || countDigits(token) < 1 || !is_digit(token[token.size() - 1]))
                    return false;
                borders.push_back(token);
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
        if (all_of(s.begin(), s.end(), is_digit) || countDigits(s) < 1 || !is_digit(s[s.size() - 1]))
            return false;
        borders.push_back(s);
    }
    if (territoryName.size() != continentName.size())
        return false;
    return true;
}

/*This method will parse through the [territories] section of the file and return the data as vector of tuples of strings and integers.
The method is only called if the file has been considered valid.*/
vector<tuple<string, int>> ConquestFileReader::parseCountries(string text) {
    vector<string> tokens;
    vector<string> territoryName;
    vector<string> continentName;
    vector<tuple<string, int>> parsedCountries;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    tokens.erase(tokens.begin());
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
        string delimiter = ",";
        size_t pos = 0;
        string token;
        loopNum = 0;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if (loopNum == 0) 
                territoryName.push_back(token);
            else if (loopNum == 3)
                continentName.push_back(token);
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
    }

    int continentNum;
    for (int i = 0; i < territoryName.size(); i++) {
        for (int j = 0; j < continentsList.size(); j++) {
            if (get<0>(continentsList[j]) == continentName[i]) {
                continentNum = get<1>(continentsList[j]);
                break;
            }
        }
        tuple<string, int> tupleCountries = make_tuple(territoryName[i], continentNum);
        parsedCountries.push_back(tupleCountries);
    }
    numOfCountries = parsedCountries.size();
    return parsedCountries;
}

/*This method will parse through the [territories] section of the file to get the borders. It will return the data as vector
of vector of integers. The method is only called if the file has been considered valid.*/
vector<vector<int>> ConquestFileReader::parseBorders(string text) {
    vector<string> tokens;
    vector<vector<int>> parsedBorders;
    int numDigits;

    stringstream check1(text);
    string intermediate;
    while (getline(check1, intermediate, '\n')) {
        tokens.push_back(intermediate);
    }
    tokens.erase(tokens.begin());
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
        string delimiter = ",";
        size_t pos = 0;
        string token;
        loopNum = 0;
        vector<int> borders;
        borders.push_back(i+1); //ID
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            if(loopNum > 3) {
                numDigits = countDigits(token);
                token = token.substr(token.length()-numDigits);
                try {
                    borders.push_back(stoi(token)); 
                } 
                catch (...) {
                }
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        numDigits = countDigits(s);
        s = s.substr(s.length()-numDigits-1);
        try {
            borders.push_back(stoi(s));
        } 
        catch (...) {
        }
        parsedBorders.push_back(borders);
    }
    return parsedBorders;
}

/*read() method for the ConquestFileReader class*/
vector<string> ConquestFileReaderAdapter::read() {
    return this->conquest_->read();
}

/*checkFormat(vector<string>) method for the ConquestFileReader class*/
bool ConquestFileReaderAdapter::checkFormat(vector<string> text) {
    return this->conquest_->checkFormat(text);
}

/*getNumOfCountries() method for the ConquestFileReader class*/
int ConquestFileReaderAdapter::getNumOfCountries() {
    return this->conquest_->getNumOfCountries();
}

/*getNumOfContinents() method for the ConquestFileReader class*/
int ConquestFileReaderAdapter::getNumOfContinents() {
    return this->conquest_->getNumOfContinents();
}

/*parseContinents(string) method for the ConquestFileReader class*/
vector<tuple<string, int>> ConquestFileReaderAdapter::parseContinents(string text) {
    return this->conquest_->parseContinents(text);
}

/*parseCountries(string) method for the ConquestFileReader class*/
vector<tuple<string, int>> ConquestFileReaderAdapter::parseCountries(string text) {
    return this->conquest_->parseCountries(text);
}

/*parseBorders(string) method for the ConquestFileReader class*/
vector<vector<int>> ConquestFileReaderAdapter::parseBorders(string text) {
    return this->conquest_->parseBorders(text);
}