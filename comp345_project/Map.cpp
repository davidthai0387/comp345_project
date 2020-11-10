#include "Player.h"
#include <iostream>
#include <vector>
using namespace std;

//---Country methods---

/*Class constructor*/
Country::Country(int num_, string name_, int continentNum_)
	: num(num_), name(name_), continentNum(continentNum_), player(), armies(0)
{
}

/*Class copy constructor*/
Country::Country(const Country& country)
{
	num = country.num;
	name = country.name;
	continentNum = country.continentNum;
	player = country.player;
	armies = country.armies;
}

/*Class default constructor*/
Country::Country()
	: num(0), name(""), player(), armies(0)
{
}

/*Class destructor*/
Country::~Country()
{

}

/*Returns the country number*/
int Country::getNum()
{
	return num;
}

/*Returns the country name*/
string Country::getName()
{
	return name;
}

/*Returns the continent number*/
int Country::getContinentNum()
{
	return continentNum;
}

/*Returns the owning player*/
Player* Country::getPlayer()
{
	return player;
}

/*Changes the owning player*/
void Country::setPlayer(Player* newPlayer)
{
	player = newPlayer;
}

/*Return the number of armies*/
int Country::getArmies()
{
	return armies;
}

/*Changes the number of armies*/
void Country::setArmies(int newNum)
{
	armies = newNum;
}

//---Continent methods---

/*Class constructor*/
Continent::Continent(string name_, int controlValue_)
	: name(name_), controlValue(controlValue_)
{
}

/*Class copy constructor*/
Continent::Continent(const Continent& continent)
{
	name = continent.name;
	controlValue = continent.controlValue;
	for (auto country : continent.countries) {
		Country* tempCountry = country;
		countries.push_back(tempCountry);
	}
}

/*Class default constructor*/
Continent::Continent()
	: name(""), controlValue(0)
{
}

/*Class destructor*/
Continent::~Continent()
{
	for (auto country : countries) {
		delete country;
	}
}

/*Returns the continent name*/
string Continent::getName()
{
	return name;
}

/*Returns the continent control value*/
int Continent::getControlValue()
{
	return controlValue;
}

/*Returns the vector of countries*/
vector<Country*> Continent::getCountries()
{
	return countries;
}

/*Adds a country to the continent*/
void Continent::add(Country* country)
{
	countries.push_back(country);
}

//---Map methods---

/*Class constructor*/
Map::Map(vector<tuple<string, int>> continents_, int numOfContinents_, vector<tuple<string, int>> countries_, int numOfCountries_, vector<vector<int>> borders_)
{
	numOfContinents = numOfContinents_;
	numOfCountries = numOfCountries_;

	//create countryNames vector
	for (int i = 0; i < numOfCountries_; i++) {
		string currentCountryName = get<0>(countries_[i]);
		countryNames.push_back(currentCountryName);
	}

	//create continents vector
	for (int i = 0; i < numOfContinents_; i++) {
		Continent* currentContinent = new Continent(get<0>(continents_[i]), get<1>(continents_[i]));
		continents.push_back(currentContinent);
	}

	//create countryContinents vector
	for (int i = 0; i < numOfCountries_; i++) {
		int currentContinentNum = get<1>(countries_[i]) - 1;
		countryContinents.push_back(currentContinentNum);
	}

	//create countries vector
	for (int i = 0; i < numOfCountries_; i++) {
		Country* country = new Country(i, countryNames[i], countryContinents[i]);
		countries.push_back(country);
		continents[country->getContinentNum()]->add(country);
	}

	//create borders vector
	borders.resize(numOfCountries);
	for (int i = 0; i < numOfCountries_; i++) {
		for (int j = 0; j < borders_[i].size(); j++) {
			int currentDestNum = borders_[i][j] - 1;
			borders[i].push_back(currentDestNum);
		}
	}
}

/*Class copy constructor*/
Map::Map(const Map& map)
{
	for (auto country : map.countries) {
		Country* tempCountry = country;
		countries.push_back(tempCountry);
	}
	for (auto name : map.countryNames) {
		string tempCountryName = name;
		countryNames.push_back(tempCountryName);
	}
	for (auto continent : map.countryContinents) {
		int tempCountryContinent = continent;
		countryContinents.push_back(tempCountryContinent);
	}
	for (auto continent : map.continents) {
		Continent* tempContinent = continent;
		continents.push_back(tempContinent);
	}
	for (int i = 0; i < borders.size(); i++) {
		vector<int> countryBorders;
		for (auto border : map.borders[i]) {
			int tempNum = border;
			countryBorders.push_back(border);
		}
		borders.push_back(countryBorders);
	}
	numOfContinents = map.numOfContinents;
	numOfContinents = map.numOfCountries;
}

/*Class default constructor*/
Map::Map()
{
}

/*Returns the vector of countries in order*/
std::vector<Country*> Map::getCountries()
{
	return countries;
}

/*Returns the vector of country names in order*/
std::vector<std::string> Map::getCountryNames()
{
	return countryNames;
}

/*Returns the the vector of continent number for the corresponding country index*/
std::vector<int> Map::getCountryContinents()
{
	return countryContinents;
}

/*Returns the vector of all continents in order*/
std::vector<Continent*> Map::getContinents()
{
	return continents;
}

/*Returns vector of border countries for each corresponding country index*/
std::vector<std::vector<int>> Map::getBorders()
{
	return borders;
}

/*Demonstrates that the map is a connected graph*/
void Map::validate()
{
	dfs();
	cout << endl;
	for (int i = 0; i < numOfContinents; i++) {
		int currentNumOfCountries = continents[i]->getCountries().size();
		dfsContinent(i, currentNumOfCountries);
		cout << endl;
	}
}

/*Shows all contents of the map*/
void Map::showContents()
{
	cout << "Borders:" << endl;
	for (int i = 0; i < numOfCountries; i++) {
		cout << i << ":" << countryNames[i] << " ---> ";
		for (auto neighbor : borders[i]) {
			cout << neighbor /*->getNum() << " "*/;
		}
		cout << endl;
	}
	cout << endl;

	cout << "Continents:" << endl;
	for (int i = 0; i < numOfContinents; i++) {
		cout << continents[i]->getName() << " ---> ";
		for (auto country : continents[i]->getCountries()) {
			cout << country->getNum() << ":" << country->getName() << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "countryContinents:" << endl;
	for (int i = 0; i < numOfCountries; i++) {
		cout << i << " " << countryContinents[i] << endl;
	}
}

//demonstrates that the map is a connected graph by performing a dfs
void Map::dfs()
{
	cout << "PERFORMING DFS ON ALL COUNTRIES OF THE MAP:" << endl;

	//no country visited initially
	bool* visited = new bool[numOfCountries];
	for (int i = 0; i < numOfCountries; i++)
		visited[i] = false;

	dfsHelper(0, visited);
}

//called by dfs()
void Map::dfsHelper(int cv, bool visited[])
{
	visited[cv] = true;
	cout << cv << ":" << countryNames[cv] << endl;

	for (auto borderCountry : borders[cv]) {
		if (!visited[borderCountry])
			dfsHelper(borderCountry, visited);
	}
}

//demonstrates that each continent is a connected subgraph by performing a dfs on each continent
void Map::dfsContinent(int currentContinentNum, int currentNumOfCountries)
{
	cout << "PERFORMING DFS ON " << continents[currentContinentNum]->getName() << endl;

	bool* visitedCountryNums = new bool[numOfCountries];
	for (int i = 0; i < numOfCountries; i++)
		visitedCountryNums[i] = false;

	dfsContinentHelper(continents[currentContinentNum]->getCountries()[0]->getNum(), currentContinentNum, visitedCountryNums);
}

//called by dfsContinent()
void Map::dfsContinentHelper(int cv, int currentContinentNum, bool visitedCountryNums[])
{
	visitedCountryNums[cv] = true;
	cout << cv << ":" << countryNames[cv] << endl;

	for (auto borderCountry : borders[cv]) {
		if (countryContinents[borderCountry] == currentContinentNum) {
			if (!visitedCountryNums[borderCountry])
				dfsContinentHelper(borderCountry, currentContinentNum, visitedCountryNums);

		}
	}
}



