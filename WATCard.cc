#include "WATCard.h"

// constructor
WATCard::WATCard() {
	currentAmount = 0;
}

// deposit money into the watcard
void WATCard::deposit( unsigned int amount ) {
	currentAmount += amount;
}

// withdraw money from the watcard
void WATCard::withdraw( unsigned int amount ) {
	currentAmount -= amount;
}

// get the watcard's balance
unsigned int WATCard::getBalance() {
	return currentAmount;
}