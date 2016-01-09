#include "Groupoff.h"
#include "Printer.h"
#include "WATCard.h"
#include "MPRNG.h"

#include <vector>
#include <iostream>

using namespace std;

extern MPRNG mprng;

void Groupoff::main() {
	printer->print( Printer::Kind::Groupoff, 'S' ); // groupoff starts

	for ( unsigned int i = 0; i < numStudents; i++ ) {
		_Accept( ~Groupoff ) {
			return;
		} or _Accept( giftCard ) {

		}
	}

	for ( unsigned int counter = 0; counter < numStudents; counter++ ) {
		_Accept( ~Groupoff ) {
			break;
		} _Else {
			yield( groupoffDelay ); // delay before gift

			WATCard *watCard = new WATCard(); // create the gift card

			watCard->deposit( sodaCost ); // deposit the gift

			int i;
			for ( ; ; ) {
				i = mprng( 0, numStudents - 1 ); // lucky student

				if ( giftCardAvailable[i] == 0 ) {
					break;
				}
			}
		
			// if ( giftCards[i].available() ) {
			// 	delete giftCards[i];

			// 	giftCards[i].reset();
			// }

			giftCards[i].delivery( watCard ); // give the card to the student
			giftCardAvailable[i] = 1;

			printer->print( Printer::Kind::Groupoff, 'D', sodaCost ); // gift given
		}
	}
}

// clean up dynamically allocated arrays
Groupoff::~Groupoff() {
	printer->print( Printer::Kind::Groupoff, 'F' ); // groupoff is done

	for ( vector<WATCard::FWATCard>::iterator it = giftCards.begin(); it != giftCards.end(); it++ ) {
		if ( it->available() ) {
			delete (*it)();
		}
	}
}

// constructor
Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ): printer( &prt ), numStudents( numStudents ), sodaCost( sodaCost ), groupoffDelay( groupoffDelay ) {

}

// student wants a giftcard
WATCard::FWATCard Groupoff::giftCard() {
	WATCard::FWATCard fWatCard;

	giftCards.push_back( fWatCard );
	giftCardAvailable.push_back( 0 );

	return fWatCard;
}