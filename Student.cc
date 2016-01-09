#include "Student.h"
#include "Printer.h"
#include "NameServer.h"
#include "WATCardOffice.h"
#include "Groupoff.h"
#include "WATCard.h"
#include "MPRNG.h"
#include "uFuture.h"

#include <iostream>
using namespace std;

extern MPRNG mprng;

void Student::main() {
	unsigned int numBottleToPurchase = mprng( 1, maxPurchases ); // student will purchase this much bottles

	unsigned int flavour = mprng( 0, 3 ); // student will get this flavour only

	printer->print( Printer::Kind::Student, id, 'S', flavour, numBottleToPurchase ); // student starts

	WATCard::FWATCard watCard = watCardOffice->create( id, 5 ); // first time creating a watcard
	
	WATCard::FWATCard giftCard = groupoff->giftCard(); // get the giftcard from groupoff

	VendingMachine *vendingMachine = nameServer->getMachine( id ); // get a vending machine from the server

	printer->print( Printer::Kind::Student, id, 'V', vendingMachine->getId() ); // student got a vending machine

	WATCard *card = NULL;
	
	yield( mprng( 1, 10 ) );

	for ( unsigned int i = 0; i < numBottleToPurchase; ) {
		try {
			// wait for watcard or gift card to be available
			_Select( watCard ) {
				card = watCard();

				vendingMachine->buy( ( VendingMachine::Flavours ) flavour, *card ); // buy a drink using the gift card

				printer->print( Printer::Kind::Student, id, 'B', card->getBalance() ); // student used watcard to buy a drink

				i++;

				yield( mprng( 1, 10 ) );
			} or _Select( giftCard ) {
				card = giftCard();

				vendingMachine->buy( ( VendingMachine::Flavours ) flavour, *card ); // buy a drink using the gift card

				printer->print( Printer::Kind::Student, id, 'G', card->getBalance() ); // student used the gift card

				delete card;
				giftCard.reset(); // this gift card is now unusable

				i++;

				yield( mprng( 1, 10 ) );
			}
		} catch ( WATCardOffice::Lost lost ) { // watcard was lost
			printer->print( Printer::Kind::Student, id, 'L' ); // watcard was lost

			// delete card;
			watCard.reset();

			watCard = watCardOffice->create( id, 5 );
		} catch ( VendingMachine::Funds funds ) { // not enough money on this watcard
			watCard = watCardOffice->transfer( id, 5 + vendingMachine->cost(), card );
		} catch ( VendingMachine::Stock stock ) { // no more drink in this machine
			vendingMachine = nameServer->getMachine( id ); // get another machine

			printer->print( Printer::Kind::Student, id, 'V', vendingMachine->getId() ); // student got a vending machine
		}
	}

	printer->print( Printer::Kind::Student, id, 'F' ); // student ends
	
	try {
		if ( watCard.available() ) {
			card = watCard();

			delete card;
		}
	} catch( WATCardOffice::Lost lost ) {
		watCard.reset();
	}
}

// constructor
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases ): printer( &prt ), nameServer( &nameServer ), watCardOffice( &cardOffice ), groupoff( &groupoff ), id( id ), maxPurchases( maxPurchases ) {
	
}