#include "PlayerStrategies.h"


// ----------HUMAN PLAYER----------

void HumanPlayer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {

	// TODO
}

vector<Country*> HumanPlayer::toAttack(Player* p) {

	// Initalize variables
	vector<Country*> potential;
	vector<Country*> confirmed;
	int counter = 1;
	int choice;

	
	// Potential list initialization
	for (Country* country : p->getOwnedCountries()) {
		potential.insert(potential.end(), country->getBorders().begin(), country->getBorders().end());
	}

	while (potential.size() > 0) {
		try {
			// Printing confirmed countries if there are any
			if (confirmed.size() != 0) {
				cout << "\nList of countries to attack:" << endl;
				int confirmedcounter = 1;
				for (Country* country : confirmed) {
					cout << "Country #" << confirmedcounter << "\t" << country->getName() << endl;
				}
				cout << endl;
			}

			// Printing attacking option list
			cout << "Here is a list of bordering countries:" << endl;
			int counter = 1;
			for (Country* country : potential) {
				cout << "Country #" << counter << "\t" << country->getName() << endl;
				counter++;
			}

			// Player choice
			if (confirmed.size() >= 1) {
				cout << "If you want to stop adding more countries, enter -1 in the choice." << endl;
			}
			cout << "Please enter the number of a country that you would like to attack: ";
			cin >> choice;
			if (choice > 0) {
				choice -= 1;
				confirmed.push_back(potential[choice]);
				potential.erase(potential.begin() + choice);
				cout << "confirmed ";
			}
			else if (confirmed.size() >= 1 && choice == -1) {
				cout << "You have decided not to attack any more countries. The confirmed countries will be attacked." << endl;
				break;
			}
			else {
				cout << "That was not a valid choice. Please try again..." << endl;
				continue;
			}
		}
		catch (exception e) {
			e.what();
		}
	}
	return confirmed;
}

vector<Country*> HumanPlayer::toDefend(Player* p) {

	// Initalize variables
	vector<Country*> potential;
	vector<Country*> confirmed;
	int counter = 1;
	int choice;

	// Potential list initialization
	potential.insert(potential.end(), p->getOwnedCountries().begin(), p->getOwnedCountries().end());

	while (potential.size() > 0) {
		try {
			// Printing confirmed countries if there are any
			if (confirmed.size() != 0) {
				cout << "\nThe list of countries to defend:" << endl;
				int confirmedcounter = 1;
				for (Country* country : confirmed) {
					cout << "Country #" << confirmedcounter << "\t" << country->getName() << endl;
				}
				cout << endl;
			}

			// Printing defending option list
			cout << "Here is a list of your owned countries:" << endl;
			int counter = 1;
			for (Country* country : potential) {
				cout << "Country #" << counter << "\t" << country->getName() << endl;
				counter++;
			}

			// Player choice
			if (confirmed.size() >= 1) {
				cout << "If you want to stop adding more countries, enter -1 in the choice." << endl;
			}
			cout << "Please enter the number of a country that you would like to defend: ";
			cin >> choice;
			if (choice > 0) {
				choice -= 1;
				confirmed.push_back(potential[choice]);
				potential.erase(potential.begin() + choice);
				cout << "Your choice has been confirmed." << endl;
			}
			else if (confirmed.size() >= 1 && choice == -1) {
				cout << "You have decided not to attack any more countries. The confirmed countries will be defend." << endl;
				break;
			}
			else {
				cout << "That was not a valid choice. Please try again..." << endl;
				continue;
			}
		}
		catch (exception e) {
			e.what();
		}
	}
	return confirmed;
}

void HumanPlayer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}


// ----------AGRESSIVE COMPUTER----------

void AggressiveComputer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {
		
	if (orderName == "Deploy") {

		//p->getPlayerOrders()->add(new Deploy());
	}
	else if (orderName == "Advance") {

	}
	else if (orderName == "Bomb") {

	}
	else if (orderName == "Blockade") {

	}
	else if (orderName == "Airlift") {

	}
	else if (orderName == "Negotiate") {

	}

}

vector<Country*> AggressiveComputer::toAttack(Player* p) {

	// TODO 
	vector<Country*> potential;
	vector<Country*> confirmed;



	return vector<Country*>();
}

vector<Country*> AggressiveComputer::toDefend(Player* p) {

	// TODO

	return vector<Country*>();
}

void AggressiveComputer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}

void BenevolentComputer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {

	// TODO

}


// ----------BENEVOLENT COMPUTER----------

vector<Country*> BenevolentComputer::toAttack(Player* p) {
	// Unecessary
	return vector<Country*>();
}

vector<Country*> BenevolentComputer::toDefend(Player* p) {
	vector<Country*> out = p->getOwnedCountries();
	sort(out.begin(), out.end());

	return out;
}

void BenevolentComputer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}

void NeutralComputer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {

	// TODO

}


// ----------NEUTRAL COMPUTER----------

vector<Country*> NeutralComputer::toAttack(Player* p) {
	// Unecessary
	return vector<Country*>();
}

vector<Country*> NeutralComputer::toDefend(Player* p) {
	// Unecessary
	return vector<Country*>();
}

void NeutralComputer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {
	// Unecessary
}
