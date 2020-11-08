#pragma once
#include <string>
#include <vector>
#include <tuple>

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
	std::string getPlayer();
	void setPlayer(std::string newPlayer);
	int getArmies();
	void setArmies(int newNum);
private:
	int num;
	std::string name;
	int continentNum;
	std::string player;
	int armies;
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
	std::vector<int> getCountryContinents();
	std::vector<Continent*> getContinents();
	std::vector<std::vector<int>> getBorders();
	void validate();
	void showContents();

private:
	std::vector<Country*> countries;	//list of countries in order
	std::vector<std::string> countryNames;	//list of country names in order
	std::vector<int> countryContinents; //continent of each country in order
	std::vector<Continent*> continents;	//list of continents in order
	std::vector<std::vector<int>> borders;	//list of all borders
	int numOfContinents;
	int numOfCountries;
	void dfs();
	void dfsHelper(int cv, bool visited[]);
	void dfsContinent(int currentContinentNum, int currentNumOfCountries);
	void dfsContinentHelper(int cv, int currentContinentNum, bool visitedCountryNums[]);
};
