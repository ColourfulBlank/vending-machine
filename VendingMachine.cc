#include "VendingMachine.h"

#include <iostream>
using namespace std;

void VendingMachine::main() {
	printer->print( Printer::Kind::Vending, id, 'S', sodaCost ); // vending machine starts

	nameServer->VMregister( ( VendingMachine * ) &uThisTask() ); // register this vending machine with the name server

	for ( ; ; ) {
		try {
			_Accept( ~VendingMachine ) {
				break;
			} or _Accept( inventory ) { // truck wants to check inventory
				_Accept( ~VendingMachine ) {
					break;
				} or _Accept( restocked ) { // truck finished restocking

				}
			} or _Accept( buy ) { // student wants to buy a drink
				fundsStock = 0;
				if ( currentWATCard->getBalance() < sodaCost ) { // student doesn't have enough money
					fundsStock = 1;
				} else if ( currentStock[currentFlavour] == 0 ) { // no more soda of this flavour
					fundsStock = 2;
				} else {
					currentWATCard->withdraw( sodaCost ); // take money from student

					currentStock[currentFlavour]--; // 1 less stock for this flavour

					printer->print( Printer::Kind::Vending, id, 'B', currentFlavour, currentStock[currentFlavour] ); // student buying
				}

				checkFundsStock.signalBlock();
			}
		} catch ( uMutexFailure::RendezvousFailure ) {

		}
		
	}

	printer->print( Printer::Kind::Vending, id, 'F' ); // vending machine ends
}

// constructor
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ): printer( &prt ), nameServer( &nameServer ), id( id ), sodaCost( sodaCost ), maxStockPerFlavour ( maxStockPerFlavour ) {
	for ( int i = 0; i < 4; i++ ) { // stock starts as 0
		currentStock[i] = 0;
	}

	fundsStock = 0;
}

// clean up dynamically allocated arrays
VendingMachine::~VendingMachine() {

}

// student wants to buy a drink of flavour
void VendingMachine::buy( Flavours flavour, WATCard &card ) {
	currentFlavour = flavour;
	currentWATCard = &card;
	checkFundsStock.wait();

	checkFundsStock.signal();
	if ( fundsStock == 1 ) { // student doesn't have enough money
		_Throw Funds();
	} else if ( fundsStock == 2 ) { // no more soda of this flavour
		_Throw Stock();
	}
}

// truck wants to check the inventory
unsigned int *VendingMachine::inventory() {
	printer->print( Printer::Kind::Vending, id, 'r' ); // truck reloading

	return currentStock;
}

// truck finished restocking
void VendingMachine::restocked() {
	printer->print( Printer::Kind::Vending, id, 'R' ); // truck finished restocking
}

// get the cost 
_Nomutex unsigned int VendingMachine::cost() {
	return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
	return id;
}