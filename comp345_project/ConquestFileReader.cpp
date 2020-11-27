#include "ConquestFileReader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <tuple>
using namespace std;

ConquestFileReader::ConquestFileReader() {
    fileName = "";
    numOfContinents = 0;
    numOfCountries = 0;    
}
ConquestFileReader::ConquestFileReader(string text) {
    fileName = text;
    numOfContinents = 0;
    numOfCountries = 0;    
}
bool isItADigit(const char value) {
    return std::isdigit(value);
}
int countDigits(string str){
  int count=0;
  for(int i=0;i<str.size();i++)
     if(isdigit(str[i])) 
        count++;
  return count;
}
int ConquestFileReader::getNumOfCountries() {
    return numOfCountries;
}
int ConquestFileReader::getNumOfContinents() {
    return numOfContinents;
}
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
int countWords(string const& str) {
    stringstream stream(str);
    return distance(istream_iterator<string>(stream), istream_iterator<string>());
}
bool ConquestFileReader::checkFiles(string text) {
    if ((text.find("image") == string::npos) || (text.find("wrap") == string::npos) || (text.find("scroll") == string::npos) 
            || (text.find("author") == string::npos) || (text.find("warn") == string::npos))
        return false;
    return true;
}
bool ConquestFileReader::checkFormat(vector<string> text) {
    if (text[0] != "")
        return false;
    if ((text[2].find("Continents") == string::npos) || (text[3].find("Territories") == string::npos))
        return false;
    if (!checkFiles(text[1]) || !checkContinents(text[2]) || !checkCountries(text[3]))
        return false;
    return true;
}
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
                if (all_of(token.begin(), token.end(), isItADigit))
                    return false;
                continents.push_back(token);
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        try {
            score.push_back(stoi(s));
        }
        catch (invalid_argument& e) {
            return false;
        }
        catch (out_of_range& e) {
            return false;
        }
    }
    if (continents.size() != score.size())
        return false;
    return true;
}
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
    for (int i = 0; i <= continents.size(); i++) {
        tuple<string, int> tupleContinents = make_tuple(continents[i], index);
        index++;
        parsedContinents.push_back(tupleContinents);
    }
    numOfContinents = parsedContinents.size();
    continentsList = parsedContinents;
    return parsedContinents;
}
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
                if (all_of(token.begin(), token.end(), isItADigit)) 
                    return false;
                territoryName.push_back(token);
            }
            else if (loopNum == 1) {
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
            else if (loopNum == 2) {
                try {
                    yCoordinate.push_back(stoi(token));
                }
                catch (invalid_argument& e) {
                    return false;
                }
                catch (out_of_range& e) {
                    return false;
                }
            }
            else if (loopNum == 3) 
                continentName.push_back(token);
            else {
                if (all_of(token.begin(), token.end(), isItADigit) || countDigits(token) < 1) 
                    return false;
                borders.push_back(token);
            }
            s.erase(0, pos + delimiter.length());
            loopNum++;
        }
        if (all_of(s.begin(), s.end(), isItADigit) || countDigits(s) < 1)
            return false;
        borders.push_back(s);
    }
    if (territoryName.size() != continentName.size())
        return false;
    return true;
}
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
                catch (invalid_argument& e) {
                }
                catch (out_of_range& e) {
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
        catch (invalid_argument& e) {
        }
        catch (out_of_range& e) {
        }
        parsedBorders.push_back(borders);
    }
    return parsedBorders;
}
