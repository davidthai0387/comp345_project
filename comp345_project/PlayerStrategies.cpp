#include "PlayerStrategies.h"


// ----------HUMAN PLAYER----------

void HumanPlayer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {

	// TODO
}

vector<Country*> HumanPlayer::toAttack(Player* p) {

	// TODO

	return vector<Country*>();
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


// ----------BENEVOLENT COMPUTER----------

void BenevolentComputer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {

	if (orderName == "Deploy") {

		while (pool > 0) {

		}

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


// ----------NEUTRAL COMPUTER----------

void NeutralComputer::issueOrder(string orderName, Player* p, vector<Player*> o, int pool, Map* m) {

	// TODO

}

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
