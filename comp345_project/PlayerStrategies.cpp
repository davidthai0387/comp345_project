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

	// Printing confirmed countries if there are any
	if (confirmed.size() != 0) {
		cout << "\nHere is the list of countries you would like to attack:" << endl;
		for (Country* country : confirmed) {
			cout << "To Attack: " << country->getName() << endl;
		}
	}
	// Printing bordering countries
	cout << "Here is a list of bordering countries:" << endl;
	for (Country* country : p->getOwnedCountries()) {
		for (Country* border : country->getBorders()) {
			cout << "Country #" << counter << " " << border->getName() << endl;
			counter++;
		}
		potential.insert(potential.end(), country->getBorders().begin(), country->getBorders().end());
	}
	
	// Player choice
	while (potential.size() > 0) {
		try {
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

	// TODO

	return vector<Country*>();
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

	// TODO

	return vector<Country*>();
}

vector<Country*> BenevolentComputer::toDefend(Player* p) {

	// TODO

	return vector<Country*>();
}

void BenevolentComputer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}

void NeutralComputer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {

	// TODO

}


// ----------NEUTRAL COMPUTER----------

vector<Country*> NeutralComputer::toAttack(Player* p) {

	// TODO

	return vector<Country*>();
}

vector<Country*> NeutralComputer::toDefend(Player* p) {

	// TODO

	return vector<Country*>();
}

void NeutralComputer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}
