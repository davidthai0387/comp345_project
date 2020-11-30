#include "PlayerStrategies.h"


// ----------HUMAN PLAYER----------

void HumanPlayer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

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

void AggressiveComputer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {
		
	if (orderName == "Deploy") {
		int armiesToDeploy = p->getNumOfArmies();
		p->setNumOfArmies(armiesToDeploy);
		vector<Country*> potentialTargets = toDefend(p);
		Country* target = potentialTargets[0];

		p->getPlayerOrders()->add(new Deploy(p, armiesToDeploy, target, m));
	}
	else if (orderName == "Advance") {
		vector<Country*> toDefendCountries = toDefend(p);
		Country* sourceCountry = toDefendCountries[0];
		Country* destCountry;
		for (Country* c : m->getCountries()) {
			if (c->getArmies() < sourceCountry->getArmies() && c->getPlayer() != p) {
				p->getPlayerOrders()->add(new Airlift(p, 111, new Country(), new Country(), m, d));
				break;
			}
		}
	}
	else if (orderName == "Bomb") {		// bombs a random country
		int targetNum = toAttack(p)[rand() % m->getCountries().size()]->getNum();

		p->getPlayerOrders()->add(new Bomb(p, m->getCountries()[targetNum], m));
	}
	else if (orderName == "Reinforcement") {	// reinforces strongest country
		int armiesToDeploy = 5;
		vector<Country*> potentialTargets = toDefend(p);
		Country* target = potentialTargets[0];

		p->getPlayerOrders()->add(new Deploy(p, armiesToDeploy, target, m));
	}
	else if (orderName == "Airlift") {		// advances strongest country's armies to a weaker enemy country
		vector<Country*> toAttackCountries = toAttack(p);
		Country* sourceCountry = toAttackCountries[0];
		Country* destCountry;
		for (Country* c : m->getCountries()) {
			if (c->getArmies() < sourceCountry->getArmies() && c->getPlayer() != p) {
				p->getPlayerOrders()->add(new Airlift(p, 111, new Country(), new Country(), m, d));
				break;
			}
		}
	}
	else if (orderName == "Card") {
		for (int i = 0; i < p->getHand().size(); i++) {
			if (p->getHand()[i]->getCardName() == "Bomb") {
				p->getHand()[i]->play(p, o, m, d, i);
			}
			if (p->getHand()[i]->getCardName() == "Airlift") {
				p->getHand()[i]->play(p, o, m, d, i);
			}
			if (p->getHand()[i]->getCardName() == "Reinforcement") {
				p->getHand()[i]->play(p, o, m, d, i);
			}
		}
		p->setCardPhaseIsOver(true);
	}

}

vector<Country*> AggressiveComputer::toAttack(Player* p) {

	// TODO 
	vector<Country*> potential;
	vector<Country*> confirmed;



	return vector<Country*>();
}

vector<Country*> AggressiveComputer::toDefend(Player* p) {

	vector<Country*> out;

	for (Country* c : p->getOwnedCountries()) {		// get countries that have armies
		if (c->getArmies() > 0) {
			out.push_back(c);
		}
	}

	if (out.size() == 0) {		// if all countries have 0 armies
		for (Country* c : p->getOwnedCountries()) {		// choose owned countries that have a neighboring enemy country
			for (Country* n : c->getBorders()) {
				if (n->getPlayer() != p) {
					out.push_back(c);
					break;
				}
			}
		}
	}
	else {		// if some countries have armies, sort those countries by number of armies and reverse (index 0 should have country with most armies)
		sort(out.begin(), out.end());
		reverse(out.begin(), out.end());
	}

	return out;
}

void AggressiveComputer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}


// ----------BENEVOLENT COMPUTER----------

void BenevolentComputer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

	if (orderName == "Deploy") {
		vector<Country*> potentialTargets = toDefend(p);
		int armiesToDeploy;
		int beginArmies = (*potentialTargets.begin())->getArmies();
		int endArmies = (*potentialTargets.end())->getArmies();
		if (beginArmies == endArmies) {
			armiesToDeploy = 1;
		}
		else {
			bool enoughArmiesToBalance = (endArmies - beginArmies) <= p->getNumOfArmies();
			armiesToDeploy = ((p->getNumOfArmies() - (*potentialTargets.end())->getArmies())) >= 0;
		}

		Country* target = potentialTargets[0];

		p->getPlayerOrders()->add(new Deploy(p, armiesToDeploy, target, m));
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

vector<Country*> BenevolentComputer::toAttack(Player* p) {
	// uncessary since Benevolent never advances
}

vector<Country*> BenevolentComputer::toDefend(Player* p) {
	static vector<Country*> out;
	if (out.size() == 0) {
		out = p->getOwnedCountries();
		sort(out.begin(), out.end());
	}
	else {
		out.erase(out.begin());
	}

	return out;
}

void BenevolentComputer::playCard(Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}


// ----------NEUTRAL COMPUTER----------

void NeutralComputer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO

}

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
