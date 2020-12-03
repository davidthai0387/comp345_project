#include "PlayerStrategies.h"
#include <cstdlib>



// ----------HUMAN PLAYER----------

void HumanPlayer::issueOrder(string orderName, Player* p, vector<Player*> o, Deck* d, Map* m) {

	if (orderName == "Bomb") {
		int terr;
		int counter = 1;
		Country* target;
		vector<Country*> potential;
		while (true) {
			try {
				for (Country* country : m->getCountries()) {
					if (country->getPlayer() != p) {
						cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
						potential.push_back(country);
						counter++;
					}
				}
				cout << "Where would you like bomb? ";
				cin >> terr;
				if (terr <= 0 || terr > potential.size())
					throw exception();
				else {
					terr -= 1;
					target = potential[terr];
					break;
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
		Country* target = toAttack(p)[0];	// shows the user his reachable countries and returns target country

		// creates the vector of countries that were deployed to this turn
		vector<Country*> deployed;
		for (Orders* o : p->getPlayerOrders()->getList()) {
			if (o->getName() == "Deploy") {
				Deploy* d = dynamic_cast<Deploy*>(o);
				if (d->getArmy() > 0) {
					deployed.push_back(d->getCountry());
				}
			}
		}

		// display potential source countries
		cout << "Here is a list of your countries that can reach your destination: " << endl;
		int counter = 1;
		vector<Country*> potentialSources;
		for (Country* country : target->getBorders()) {
			if (country->getPlayer()->getName() == p->getName() && country->getArmies() > 0 || find(deployed.begin(), deployed.end(), country) != deployed.end()) {		// check if it's an ally country and there are armies on it
				if (find(deployed.begin(), deployed.end(), country) != deployed.end()) {		// check if this country has been deployed to this turn
					int additionalArmies = 0;
					for (Country* d : deployed) {		// loop through all deployed countries
						if (d == country) {		// find the country that is currently being displayed
							for (Orders* o : p->getPlayerOrders()->getList()) {		// loops through all orders
								if (o->getName() == "Deploy") {		// find only deploy orders
									Deploy* deployOrder = dynamic_cast<Deploy*>(o);
									if (deployOrder->getCountry() == d) {		// if the deploy order target is the country being deployed
										additionalArmies += deployOrder->getArmy();		// keep count of the numbers of armies deployed to this country
									}
								}
							}
						}
					}
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << " (" << country->getArmies() + additionalArmies << " after deploy order execution)" << endl;
				}
				else {
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
				}

				potentialSources.push_back(country);
				counter++;
			}
		}

		// user chooses source country
		cout << "Enter the number of the country that you want to advance from: ";
		int src;
		while (true) {
			try {
				cin >> src;
				if (src <= 0 || src > potentialSources.size()) {
					throw exception();
				}
				else {
					src--;
					break;
				}
					
			}
			catch (...) {
				cout << "That is not a valid input. Please try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}

		// user chooses the number of armies to advance
		cout << "How many armies would you like to advance: ";
		int army;
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
		p->getPlayerOrders()->add(new Advance(p, army, potentialSources[src], target, m, d));
		p->setAdvancePhaseIsOver(true);

	}
	else if (orderName == "Blockade") {
		int terr;
		int counter = 1;
		Country* target;
		while (true) {
			try {
				for (Country* country : p->getOwnedCountries()) {
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
					counter++;
				}
				cout << "Where would you like to form a blockade? ";
				cin >> terr;
				if (terr <= 0 || terr > p->getOwnedCountries().size())
					throw exception();
				terr -= 1;
				target = p->getOwnedCountries()[terr];
				break;
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
				if (terr1 <= 0 || terr1 > p->getOwnedCountries().size())
					throw exception();
				terr1 -= 1;
				target1 = p->getOwnedCountries()[terr1];
				break;
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
				if (terr2 <= 0 || terr2 > potential.size())
					throw exception();
				terr2 -= 1;
				target2 = potential[terr2];
				break;
			}
			catch (...) {
				cout << "There is no such country that you own... Try again..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		
		while (true) {
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
					cout << "Player #" << counter << "\t" << player->getName() << endl;
					counter++;
				}
				cout << "Which player would you like to negociate with (Enter the player number)? ";
				cin >> playernumber;
				if (playernumber <= 0 || playernumber > o.size())
					throw exception();
				playernumber -= 1;
				if (o[playernumber] != p) {
					targetplayer = o[playernumber];
					break;
				}
				else
					throw exception();
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
				if (terr <= 0 || terr > p->getOwnedCountries().size())
					throw exception();
				terr -= 1;
				target = p->getOwnedCountries()[terr];
				break;
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
					while (true) {
						try {
							cardchoice -= 1;
							p->getHand()[cardchoice]->play(p, o, m, d, cardchoice);
							cout << "Would you like to continue playing more cards?(y/n) ";
							cin >> cont;

							if (cont == "n") {
								p->setCardPhaseIsOver(1);
								break;
							}
							else if (cont != "y")
								throw exception();
						}
						catch (...) {
							cout << "That is not a valid option. Please try again..." << endl;
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
						}
					}
					if (cont == "n")
						break;

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

	// get all countries that are reachable (owned and enemy)
	vector<Country*> reachable;
	for (Country* country : p->getOwnedCountries()) {
		if (country->getArmies() > 0) {
			for (Country* border : country->getBorders()) {
				if (find(reachable.begin(), reachable.end(), border) == reachable.end())
					reachable.push_back(border);
			}
		}

	}
	// now reachable contains all countries that are reachable

	// adding borders of deploy country
	for (Orders* order : p->getPlayerOrders()->getList()) {
		if (order->getName() == "Deploy") {
			Deploy* d = dynamic_cast<Deploy*>(order);
			for (Country* border : d->getCountry()->getBorders()) {
				if (find(reachable.begin(), reachable.end(), border) == reachable.end())
					reachable.push_back(border);
			}
		}
	}

	// separating enemy and own countries
	vector<Country*> ownCountries;
	vector<Country*> enemyCountries;
	for (Country* country : reachable) {
		if (country->getPlayer() == p)
			ownCountries.push_back(country);
		else
			enemyCountries.push_back(country);
	}

	// appending enemies to allies in reachable
	reachable = ownCountries;
	reachable.insert(reachable.end(), enemyCountries.begin(), enemyCountries.end());

	// display enemyCountries and ownCountries
	int counter = 1;
	cout << "Here is a list of reachable countries:" << endl;
	cout << "Ally countries:" << endl;
	for (Country* ally : ownCountries) {
		cout << "Country #" << counter << "\t" << ally->getName() << "\tArmies: " << ally->getArmies() << endl;
		counter++;
	}
	cout << "Enemy countries:" << endl;
	for (Country* enemy : enemyCountries) {
		cout << "Country #" << counter << "\t" << enemy->getName() << "\tArmies: " << enemy->getArmies() << endl;
		counter++;
	}

	// user chooses target Country
	cout << "Enter the number of the country that you would like to advance to: ";
	int target;
	while (true) {
		try {
			cin >> target;
			if (target <= 0 || target > reachable.size()) {
				throw exception();
			}
			else {
				target--;
				break;
			}
		}
		catch (...) {
			cout << "That is not a valid input. Please try again..." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	vector<Country*> out;
	out.push_back(reachable[target]);
	return out;
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
			// creates the vector of countries that were deployed to this turn
			vector<Country*> deployed;
			for (Orders* o : p->getPlayerOrders()->getList()) {
				if (o->getName() == "Deploy") {
					Deploy* d = dynamic_cast<Deploy*>(o);
					if (d->getArmy() > 0) {
						deployed.push_back(d->getCountry());
					}
				}
			}
			for (Country* country : potential) {
				if (find(deployed.begin(), deployed.end(), country) != deployed.end()) {		// check if it's an ally country and there are armies on it
					int additionalArmies = 0;
					for (Country* d : deployed) {		// loop through all deployed countries
						if (d == country) {		// find the country that is currently being displayed
							for (Orders* o : p->getPlayerOrders()->getList()) {		// loops through all orders
								if (o->getName() == "Deploy") {		// find only deploy orders
									Deploy* deployOrder = dynamic_cast<Deploy*>(o);
									if (deployOrder->getCountry() == d) {		// if the deploy order target is the country being deployed
										additionalArmies += deployOrder->getArmy();		// keep count of the numbers of armies deployed to this country
									}
								}
							}
						}
					}
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << " (" << country->getArmies() + additionalArmies << " after deploy order execution)" << endl;
				}
				else {
					cout << "Country #" << counter << "\t" << country->getName() << "\tArmies: " << country->getArmies() << endl;
				}
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
		// no need to advance since all the armies are already deployed equally to each country
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
