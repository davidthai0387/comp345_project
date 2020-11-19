#pragma once

#include <string>
#include <vector>
#include <tuple>

using namespace std;

class Player;
//Nodes of the adjacency list
class Country {
public:
	Country(int num_, std::string name_, int continentNum_);
	Country(const Country& country);
	Country();
	~Country();
	int getNum();
	std::string getName();
	int getContinentNum();
	Player* getPlayer();
	void setPlayer(Player* newPlayer);
	int getArmies();
	void setArmies(int newNum);
	vector<Country*> getBorders();
	void setBorders(vector<Country*> b);
	void addBorder(Country* c);
private:
	int num;
	std::string name;
	int continentNum;
	Player* player;
	int armies;
	vector<Country*> borders;
};

//
class Continent {
public:
	Continent(std::string name_, int controlValue_);
	Continent(const Continent& continent);
	Continent();
	~Continent();
	std::string getName();
	int getControlValue();
	std::vector<Country*> getCountries();
	void add(Country* country);
private:
	std::string name;
	int controlValue;
	std::vector<Country*> countries;
};


class Map {
public:
	Map(std::vector<std::tuple<std::string, int>> continents_, int numOfContinents_, std::vector<std::tuple<std::string, int>> countries_, int numOfCountries_, std::vector<std::vector<int>> borders_);
	Map(const Map& map);
	Map();
	std::vector<Country*> getCountries();
	std::vector<std::string> getCountryNames();
	std::vector<int> getCountryContinentNumbers();
	std::vector<Continent*> getContinents();
	std::vector<std::vector<int>> getBorderNumbers();
	std::vector<std::vector<Country*>> getAllBorders();
	bool validate();
	void showContents();

private:
	std::vector<Country*> countries;	//list of countries in order
	std::vector<std::string> countryNames;	//list of country names in order
	std::vector<int> countryContinentNumber; //continent of each country in order
	std::vector<Continent*> continents;	//list of continents in order
	std::vector<std::vector<int>> borderNumbers;	//list of all borders country numbers
	std::vector<std::vector<Country*>> allBorders;	//list of all border countries
	int numOfContinents;
	int numOfCountries;
	bool dfs();
	void dfsHelper(int cv, bool visited[]);
	bool dfsContinents();
	void dfsContinentsHelper(int cv, int currentContinentNum, bool visitedCountryNums[]);
};
