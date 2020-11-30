#include "PlayerStrategies.h"
#include <cstdlib>


// ----------HUMAN PLAYER----------

void HumanPlayer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO
	if (orderName == "Bomb") {
		string terr;
		Country* target;
		cout << "Where would you like bomb? ";
		cin >> terr;
		for (Country* country : m->getCountries()) {
			if (country->getName() == terr) {
				target = country;
				break;
			}
		}
		p->getPlayerOrders()->add(new Bomb(p, target, m));
	}
	else if (orderName == "Deploy") {
		int army;
		Country* target;
		cout << "How many troops would you like to deploy? ";
		cin >> army;
		vector<Country*> defending = toDefend(p);
		target = defending[0];
		p->getPlayerOrders()->add(new Deploy(p, army, target, m));
		p->setNumOfArmies(p->getNumOfArmies()-army);
	}
	else if (orderName == "Advance") {
		vector<Country*> atk = toAttack(p);
		vector<Country*> potential;
		int army = 0;
		int counter = 1;
		for (Country* country : atk[0]->getBorders()) {
			if (country->getPlayer()->getName() == p->getName() && country->getArmies() > 0) {
				cout << "Country #" << counter << "\t" << country->getName() << endl;
				potential.push_back(country);
				counter++;
			}
		}
		cout << "How many soldiers would you like to advance with? ";
		while (army < 0) {
			try {
				cin >> army;
				if (army < 0) {
					throw exception();
				}
				break;
			}
			catch (...) {
				cout << "That is not a valid input. Please try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}

		counter -= 1;
		p->getPlayerOrders()->add(new Advance(p, army, potential[counter], atk[0], m, d));
		p->setAdvancePhaseIsOver(true);

		//here
	}
	else if (orderName == "Blockade") {
		string terr;
		Country* target;
		cout << "Where would you like to form a blockade? ";
		cin >> terr;
		for (Country* country : m->getCountries()) {
			if (country->getName() == terr) {
				target = country;
				break;
			}
		}
		p->getPlayerOrders()->add(new Blockade(p, target, m));			
	}
	else if (orderName == "Airlift") {
		int army;
		string terr1;
		string terr2;
		Country* target1;
		Country* target2;

		cout << "From which of your terriories would you deploy from? ";
		cin >> terr1;
		cout << "How many troops would you like to deploy? ";
		cin >> terr2;
		for (Country* country : m->getCountries()) {
			if (country->getName() == terr1) {
				target1 = country;
				break;
			}
		}
		for (Country* country : m->getCountries()) {
			if (country->getName() == terr2) {
				target2 = country;
				break;
			}
		}
		p->getPlayerOrders()->add(new Airlift(p, army, target1, target2, m, d));
	}
	else if (orderName == "Negotiate") {
		string playername;
		Player* targetplayer;
		cout << "Which player would you like to negociate with? ";
		cin >> playername;
		for (Player* player : o) {
			if (player->getName() == playername) {
				targetplayer = player;
				break;
			}
		}
		p->getPlayerOrders()->add(new Negotiate(p, targetplayer, m));
	}
	else if (orderName == "Reinforcement") {
		string terr;
		Country* target;
		cout << "Where would you like to reinforce? ";
		cin >> terr;
		for (Country* country : m->getCountries()) {
			if (country->getName() == terr) {
				target = country;
				break;
			}
		}
		p->getPlayerOrders()->add(new Deploy(p, 5, target, m));
		p->setNumOfArmies(p->getNumOfArmies() - 5);
	}
	else if (orderName == "Card") {
		int cardchoice;
		string cont;
		cout << "Here are your cards: " << endl;
		for (int i = 0; i < p->getHand().size(); i++){
			cout << "Card #" << i+1 << "\t" << p->getHand()[i]->getCardName() << endl;
		}
		cout << "Please choose the card number you would like to play: ";
		cin >> cardchoice;
		cardchoice -= 1;
		p->getHand()[cardchoice]->play(p, o, m, d, cardchoice);
		cout << "Would you like to continue playing more cards?(y/n) ";
		cin >> cont;
		if (cont == "n")
			p->setCardPhaseIsOver(1);
	}
}

vector<Country*> HumanPlayer::toAttack(Player* p) {

	// Initalize variables
	vector<Country*> potential;
	vector<Country*> confirmed;
	int choice;

	
	// Potential list initialization
	for (Country* country : p->getOwnedCountries()) {
		for (Country* border : country->getBorders()) {
			if(border->getPlayer()->getName() != p->getName()){
				potential.push_back(border);
			}
		}
	}
	while (true) {
		try {
			// Printing attacking option list
			cout << "Here is a list of bordering countries:" << endl;
			int counter = 1;
			for (Country* country : potential) {
				cout << "Country #" << counter << "\t" << country->getName() << endl;
				counter++;
			}

			// Player choice
			cout << "Please enter the number of a country that you would like to attack: ";
			cin >> choice;
			if (choice > 0) {
				choice -= 1;
				confirmed.push_back(potential[choice]);
				break;
			}
			else {
				throw exception();
			}
		}
		catch (exception e) {
			cout << "That was not a valid choice. Please try again..." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
	for (Country* country : p->getOwnedCountries())
		potential.push_back(country);

	while (true) {
		try {
			// Printing defending option list
			cout << "Here is a list of your owned countries:" << endl;
			int counter = 1;
			for (Country* country : potential) {
				cout << "Country #" << counter << "\t" << country->getName() << endl;
				counter++;
			}

			// Player choice
			cout << "Please enter the number of a country that you would like to defend: ";
			cin >> choice;
			if (choice > 0) {
				choice -= 1;
				confirmed.push_back(potential[choice]);
			}
			else {
				throw exception();
			}
		}
		catch (exception e) {
			cout << "That was not a valid choice. Please try again..." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

	// Variable initialization
	Country* strongest = p->getOwnedCountries()[0];
	vector<Country*> potential;
	static vector<Country*> confirmed;

	// Potential list initialization
	for (Country* country : p->getOwnedCountries()) {
		if (strongest->getArmies() <= country->getArmies())
			strongest = country;
	}

	if (confirmed.size() > 0) {
		for (Country* border : strongest->getBorders()) {
			if (border->getPlayer()->getName() != p->getName() && confirmed.size() < 4)
				confirmed.push_back(border);
		}
		if (confirmed.size() == 0) {
			confirmed.push_back(strongest->getBorders()[rand() % (strongest->getBorders().size())]);
		}
	}
	else {
		confirmed.erase(confirmed.begin());
	}
	
	return confirmed;
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
