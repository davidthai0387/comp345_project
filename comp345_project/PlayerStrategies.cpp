#include "PlayerStrategies.h"
#include <cstdlib>



// ----------HUMAN PLAYER----------

void HumanPlayer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

	// TODO
	if (orderName == "Bomb") {
		string terr;
		Country* target;
		while (true) {
			try {
				cout << "Where would you like bomb? ";
				cin >> terr;
				for (Country* country : m->getCountries()) {
					if (country->getName() == terr) {
						target = country;
						break;
					}
				}
			}
			catch (...) {
				cout << "This country does not exist on the map of this world." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
		if (army > p->getNumOfArmies()) {
			army = p->getNumOfArmies();
		}
		p->getPlayerOrders()->add(new Deploy(p, army, target, m));
		p->setNumOfArmies(p->getNumOfArmies()-army);
	}
	else if (orderName == "Advance") {
		int choice;
		vector<Country*> potential;
		int src = 0;
		int army = 0;
		int counter = 1;

		cout << "Would you like to attack an enemy country or move your troops?" << endl;
		cout << "1.\tAttack enemy country" << endl;
		cout << "2.\tMove troops" << endl;
		while (true) {
			try {
				cout << "Choice: ";
				cin >> choice;
				if (choice != 1 && choice != 2)
					throw exception();
				break;
			} catch (...){
				cout << "That is not a valid option. Please try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		if (choice == 1) {
			vector<Country*> atk = toAttack(p);
			for (Country* country : atk[0]->getBorders()) {
				if (country->getPlayer()->getName() == p->getName() && country->getArmies() > 0) {
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
					potential.push_back(country);
					counter++;
				}
			}

			for (Orders* order : p->getPlayerOrders()->getList()) {
				if (order->getName() == "Deploy") {
					Deploy* d = dynamic_cast<Deploy*>(order);
					for (Country* country : atk[0]->getBorders()) {
						if (country->getPlayer()->getName() == p->getName() && find(potential.begin(), potential.end(), country) == potential.end()) {
							cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
							counter++;
							potential.push_back(country);
						}
					}
				}
			}


			cout << "Where would you like to advance from?\nChoose the country number where you like as the source of your advance: ";
			while (true) {
				try {
					cin >> src;
					if (src < 0 && src > counter) {
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
			src -= 1;

			cout << "How many soldiers would you like to advance with? ";
			while (true) {
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
			p->getPlayerOrders()->add(new Advance(p, army, potential[src], atk[0], m, d));
			p->setAdvancePhaseIsOver(true);
		}
		else if (choice == 2) {
			cout << "Here is a list of your countries that are reachable: " << endl;
			vector<Country*> reachable;
			vector<Country*> deployed;
			for (Orders* o : p->getPlayerOrders()->getList()) {
				if (o->getName() == "Deploy") {
					Deploy* d = dynamic_cast<Deploy*>(o);
					if (d->getArmy() > 0) {
						Country* newlyDeployed = d->getCountry();
						deployed.push_back(d->getCountry());
						for (Country* border : newlyDeployed->getBorders()) {
							if (border->getPlayer() == p && find(reachable.begin(), reachable.end(), border) == reachable.end()) {
								reachable.push_back(border);
							}
						}
					}
				}
			}
			for (Country* ownedCountry : p->getOwnedCountries()) {
				for (Country* border : ownedCountry->getBorders()) {
					if (border->getPlayer() == p && border->getArmies() > 0 && find(reachable.begin(), reachable.end(), ownedCountry) == reachable.end()) {
						reachable.push_back(ownedCountry);
						break;
					}
				}
			}
			for (Country* reachableCountry : reachable) {
				cout << "Country #" << counter << "\t" << reachableCountry->getName() << "\tArmies: " << reachableCountry->getArmies() << endl;
				counter++;
			}
			counter = 1;
			cout << "Please enter the number of the country you would like to advance to: ";
			int num;
			cin >> num;
			num--;
			if (num >= reachable.size()) {
				cout << "Invalid input country" << endl;
				return;
			}
			//vector<Country*> def = toDefend(p);

			cout << "Here is a list of your countries that can reach your destination: " << endl;
			for (Country* country : reachable[num - 1]->getBorders()) {
				if (country->getPlayer()->getName() == p->getName() && country->getArmies() > 0 || find(deployed.begin(), deployed.end(), country) != deployed.end()) {
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
					potential.push_back(country);
					counter++;
				}
			}

			if (potential.size() == 0) {
				cout << "This country is currently unreachable" << endl;\
				p->setAdvancePhaseIsOver(true);
				return;
			}

			cout << "Where would you like to advance from?\nChoose the country number where you like as the source of your advance: ";

			while (true) {
				try {
					cin >> src;
					if (src < 0) {
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
			src -= 1;

			cout << "How many soldiers would you like to advance with? ";
			while (true) {
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
			p->getPlayerOrders()->add(new Advance(p, army, potential[src], reachable[num], m, d));
			p->setAdvancePhaseIsOver(true);
		}
	}
	else if (orderName == "Blockade") {
		string terr;
		Country* target;
		while (true) {
			try {
				cout << "Where would you like to form a blockade? ";
				cin >> terr;
				for (Country* country : m->getCountries()) {
					if (country->getName() == terr) {
						target = country;
					}
				}
			}
			catch (...) {
				cout << "This country does not exist on the map of this world... Please try again...";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		
		p->getPlayerOrders()->add(new Blockade(p, target, m));			
	}
	else if (orderName == "Airlift") {
		int army;
		int terr1;
		int terr2;
		Country* target1;
		Country* target2;
		vector<Country*> potential;

		while (true) {
			try {
				int counter = 1;
				for (Country* country : p->getOwnedCountries()) {
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
					counter++;
				}
				cout << "From which of your terriories would you deploy from?\nEnter the number of the country: ";
				cin >> terr1;
				terr1 -= 1;
				target1 = p->getOwnedCountries()[terr1];
			}
			catch (...) {
				cout << "There is no such country that you own... Try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		
		while (true) {
			try {
				int counter = 1;
				for (Country* country : m->getCountries()) {
					if (country->getPlayer() != p) {
						cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
						potential.push_back(country);
						counter++;
					}
				}
				cout << "Where would you like to airlift to?\nEnter the number of the country: ";
				cin >> terr2;
				terr2 -= 1;
				target2 = potential[terr2];
			}
			catch (...) {
				cout << "There is no such country that you own... Try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		
		while (army < 0) {
			try {
				cout << "How many soldiers would you like to airlift with? ";
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
		p->getPlayerOrders()->add(new Airlift(p, army, target1, target2, m, d));
	}
	else if (orderName == "Negotiate") {
		int playernumber;
		Player* targetplayer;

		while (true) {
			try {
				int counter = 1;
				for (Player* player : o) {
					cout << "Player " << counter << "\t" << player->getName() << endl;
					counter++;
				}
				cout << "Which player would you like to negociate with (Enter the player number)? ";
				cin >> playernumber;
				playernumber -= 1;
				if ((playernumber < 0 && playernumber > o.size()) || o[playernumber] != p)
					throw exception();
				for (Player* player : o) {
					if (player == o[playernumber]) {
						targetplayer = player;
						break;
					}
				}
			}
			catch (...) {
				cout << "That is not a valid player. Please try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		
		p->getPlayerOrders()->add(new Negotiate(p, targetplayer, m));
	}
	else if (orderName == "Reinforcement") {
		int terr;
		Country* target;
		while (true) {
			try {
				int counter = 1;
				for (Country* country : p->getOwnedCountries()) {
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
					counter++;
				}
				cout << "Where would you like to reinforce?\nEnter the country number: ";
				cin >> terr;
				terr -= 1;
				target = p->getOwnedCountries()[terr];
			}
			catch (...) {
				cout << "That is not a valid country. Please try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		p->getPlayerOrders()->add(new Deploy(p, 5, target, m));
		p->setNumOfArmies(p->getNumOfArmies() - 5);
	}
	else if (orderName == "Card") {
		int cardchoice;
		string cont;
		while (true) {
			try {
				cout << "Here are your cards: " << endl;
				for (int i = 0; i < p->getHand().size(); i++) {
					cout << "Card #" << i + 1 << "\t" << p->getHand()[i]->getCardName() << endl;
				}
				cout << "Please choose the card number you would like to play (Enter 0 if you would not like to play a card): ";
				cin >> cardchoice;
				if (cardchoice == 0) {
					p->setCardPhaseIsOver(1);
					break;
				}
				else if (cardchoice > p->getHand().size())
					throw exception();
				else {
					cardchoice -= 1;
					p->getHand()[cardchoice]->play(p, o, m, d, cardchoice);
					cout << "Would you like to continue playing more cards?(y/n) ";
					cin >> cont;
					if (cont == "n") {
						p->setCardPhaseIsOver(1);
						break;
					}
				}
			}
			catch (...) {
				cout << "That choice is invalid. Please try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			
		}
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
				cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
				counter++;
			}

			// Player choice
			cout << "Please enter the number of a country that you would like to attack: ";
			cin >> choice;
			if (choice > 0 && choice <= potential.size()) {
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
				cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
				counter++;
			}

			// Player choice
			cout << "Please enter the number of a country that you would like to defend: ";
			cin >> choice;
			if (choice > 0 && choice <= potential.size()) {
				choice -= 1;
				confirmed.push_back(potential[choice]);
				break;
			}
			else {
				throw exception();
			}
		}
		catch (...) {
			cout << "That was not a valid choice. Please try again..." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return confirmed;
}


// ----------AGRESSIVE COMPUTER----------

void AggressiveComputer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {
		
	if (orderName == "Deploy") {
		int armiesToDeploy = p->getNumOfArmies();
		p->setNumOfArmies(0);
		vector<Country*> potentialTargets = toDefend(p);
		Country* target = potentialTargets[0];

		p->getPlayerOrders()->add(new Deploy(p, armiesToDeploy, target, m));
	}
	else if (orderName == "Advance") {
		vector<Country*> toAdvanceCountries = toAttack(p); // Has both enemy finding and moving logic
		Country* destCountry = toAdvanceCountries[0];
		vector<Country*> ownedCountries = p->getOwnedCountries();
		Country* sourceCountry = ownedCountries[0];
		for (Country* country : p->getOwnedCountries()) {
			if (sourceCountry->getArmies() <= country->getArmies())
				sourceCountry = country;
		}
		

		int armyValue = 0;
		for (Orders* order : p->getPlayerOrders()->getList()) {
			if (order->getName() == "Deploy") {
				Deploy* d = dynamic_cast<Deploy*>(order);
				armyValue += d->getArmy();
			}
		}
		armyValue += sourceCountry->getArmies();


		p->getPlayerOrders()->add(new Advance(p, armyValue, sourceCountry, destCountry, m, d));
		p->setAdvancePhaseIsOver(true);
	}
	else if (orderName == "Bomb") {		// bombs a random country
		int targetNum = rand() % m->getCountryNames().size();

		p->getPlayerOrders()->add(new Bomb(p, m->getCountries()[targetNum], m));
	}
	else if (orderName == "Reinforcement") {	// reinforces strongest country
		int armiesToDeploy = 5;
		vector<Country*> potentialTargets = toDefend(p);
		Country* target = potentialTargets[0];
		p->getPlayerOrders()->add(new Deploy(p, armiesToDeploy, target, m));
	}
	else if (orderName == "Airlift") {		// advances strongest country's armies to a weaker enemy country
		vector<Country*> ownedCountries = p->getOwnedCountries();
		Country* sourceCountry = ownedCountries[0];
		for (Country* country : p->getOwnedCountries()) {
			if (sourceCountry->getArmies() <= country->getArmies())
				sourceCountry = country;
		}

		int armyValue = 0;
		for (Orders* order : p->getPlayerOrders()->getList()) {
			if (order->getName() == "Deploy") {
				Deploy* d = dynamic_cast<Deploy*>(order);
				armyValue += d->getArmy();
			}
		}
		armyValue += sourceCountry->getArmies();

		Country* destCountry;
		for (Country* c : m->getCountries()) {
			if (c->getArmies() < sourceCountry->getArmies() && c->getPlayer() != p) {
				destCountry = c;
				p->getPlayerOrders()->add(new Airlift(p, armyValue, sourceCountry, destCountry, m, d));
				break;
			}
		}
	}
	else if (orderName == "Card") {
		for (int i = p->getHand().size() - 1; i >= 0; i--) {
			if (p->getHand()[i]->getCardName() == "Bomb") {
				p->getHand()[i]->play(p, o, m, d, i);
				continue;
			}
			if (p->getHand()[i]->getCardName() == "Airlift") {
				p->getHand()[i]->play(p, o, m, d, i);
				continue;
			}
			if (p->getHand()[i]->getCardName() == "Reinforcement") {
				p->getHand()[i]->play(p, o, m, d, i);
				continue;
			}
		}
		p->setCardPhaseIsOver(true);
	}
}

vector<Country*> AggressiveComputer::toAttack(Player* p) {

	// Variable initialization
	vector<Country*> ownedCountries = p->getOwnedCountries();
	Country* strongest = ownedCountries[0];
	vector<Country*> confirmed;

	// Strongest initialization
	for (Country* country : p->getOwnedCountries()) {
		if (strongest->getArmies() <= country->getArmies())
			strongest = country;
	}

	// If enemies in border
	for (Country* border : strongest->getBorders()) {
		if (border->getPlayer()->getName() != p->getName()) {
			confirmed.push_back(border);
			cout << "There are enemies in the border." << "\t" << strongest->getName() << "\t" << confirmed[0]->getName() << endl;
			break;
		}
				
	}
	// If no enemies in border
	if (confirmed.size() == 0) {
		
		confirmed.push_back(strongest->getBorders()[rand() % (strongest->getBorders().size())]);
		cout << "There are no enemies in the border." << "\t" << strongest->getName() << "\t" << confirmed[0]->getName() << endl;
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
		sort((out.begin()), out.end(), CountryComparator());
		reverse(out.begin(), out.end());
	}
	cout << out[0]->getName() << endl;
	return out;
}

// ----------BENEVOLENT COMPUTER----------

void BenevolentComputer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

	if (orderName == "Deploy") {
		vector<Country*> potentialTargets = toDefend(p);
		int armiesToDeploy = 0;
		int beginArmies = potentialTargets[0]->getArmies();
		int endArmies = potentialTargets[potentialTargets.size() - 1]->getArmies();
		if (beginArmies == endArmies) {
			armiesToDeploy = 1;
		}
		else {
			bool enoughArmiesToBalance = (endArmies - beginArmies) <= p->getNumOfArmies();
			armiesToDeploy = enoughArmiesToBalance ? (endArmies - beginArmies) : p->getNumOfArmies();
		}

		Country* target = potentialTargets[0];

		p->setNumOfArmies(p->getNumOfArmies() - armiesToDeploy);

		//cout << "----- Player " << p->getName() << " deploying " << armiesToDeploy << " armies to " << target->getName() << endl;

		p->getPlayerOrders()->add(new Deploy(p, armiesToDeploy, target, m));
	}
	else if (orderName == "Advance") {
		p->setAdvancePhaseIsOver(true);
	}
	else if (orderName == "Bomb") {

	}
	else if (orderName == "Reinforcement") {

	}
	else if (orderName == "Blockade") {

	}
	else if (orderName == "Airlift") {

	}
	else if (orderName == "Negotiate") {

	}
	else if (orderName == "Card") {
		p->setCardPhaseIsOver(true);
	}

}

vector<Country*> BenevolentComputer::toAttack(Player* p) {
	// uncessary since Benevolent never advances
	return vector<Country*>();
}

vector<Country*> BenevolentComputer::toDefend(Player* p) {
	static vector<Country*> out;
	
	if (out.size() >= 1) {
		out.erase(out.begin());
	}
	if (out.size() == 0) {
		out = p->getOwnedCountries();
		sort(out.begin(), out.end(), CountryComparator());
	}

	return out;
}


// ----------NEUTRAL COMPUTER----------

void NeutralComputer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

	cout << "----- Player " << p->getName() << " is neutral and makes no move..." << endl;

	p->setNumOfArmies(0);
	p->setAdvancePhaseIsOver(true);
	p->setCardPhaseIsOver(true);

}

vector<Country*> NeutralComputer::toAttack(Player* p) {
	// Unecessary
	return vector<Country*>();
}

vector<Country*> NeutralComputer::toDefend(Player* p) {
	// Unecessary
	return vector<Country*>();
}
