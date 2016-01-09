#include "WATCardOffice.h"
#include "Printer.h"
#include "Bank.h"
#include "WATCard.h"
#include "MPRNG.h"

#include <iostream>

#include <queue>
#include <map>

using namespace std;

extern MPRNG mprng;

// watcard office
/////
// constructor
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ): printer( &prt ), bank( &bank ), numCouriers ( numCouriers ) {
// cout << "office ctr" << endl;
}

// clean up dynamically allocated arrays
WATCardOffice::~WATCardOffice() {
// cout << "office destructor" << endl;
	for ( unsigned int i = 0; i < numCouriers; i++ ) {
		delete couriers[i];
	}
	delete [] couriers;

	while ( !jobs.empty() ) {
		delete jobs.front();
		jobs.pop();
	}
}

// student wants to create a WATCard
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	Job *job = new Job( sid, amount ); // create a job for the courier

	jobs.push( job ); // put the job onto the queue

	printer->print( Printer::Kind::WATCardOffice, 'C', sid, amount ); // created watcard

	return job->result;
}

// student wants to trasnfer funds to watcard
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
	Job *job = new Job( sid, amount, card ); // create a job for the courier

	jobs.push( job ); // put the job onto the queue

	printer->print( Printer::Kind::WATCardOffice, 'T', sid, amount ); // transferred funds to watcard

	return job->result;
}

// called by courier to request a job
WATCardOffice::Job *WATCardOffice::requestWork() {
// cout << "got job in request work" << endl;
	Job *job = NULL;
	if ( !jobs.empty() ) {
		job = jobs.front(); // get the first job
		jobs.pop();

		printer->print( Printer::Kind::WATCardOffice, 'W' ); // the work is given to a courier
	}
	
	return job;
}

void WATCardOffice::main() {
	printer->print( Printer::Kind::WATCardOffice, 'S' ); // office starts

	couriers = new Courier*[numCouriers];
	for ( unsigned int i = 0; i < numCouriers; i++ ) { // create the couriers
		couriers[i] = new Courier( ( WATCardOffice* ) &uThisTask(), printer, bank, i );
	}

	for ( ; ; ) {
// cout << "in office main loop" << endl;
		_Accept( ~WATCardOffice ) { // destructor
			printer->print( Printer::Kind::WATCardOffice, 'F' ); // the office is done

			for ( unsigned int i = 0; i < numCouriers; i++ ) {
				_Accept( requestWork ) {
// cout << "courier end" << endl;
				}
			}

			break;
		} or _Accept ( create ) {

		} or _Accept ( transfer ) {

		} or _When( !jobs.empty() ) _Accept ( requestWork ) { // courier requests work
			
		}
	}
}

// job
/////
// constructor
WATCardOffice::Job::Job( unsigned int sid, unsigned int amount, WATCard *card ): sid( sid ), amount( amount ), card ( card ) {

}

// courier
/////
// constructor
WATCardOffice::Courier::Courier( WATCardOffice *watCardOffice, Printer *printer, Bank *bank, unsigned int id ): watCardOffice( watCardOffice ), printer( printer ), bank( bank ), id( id ) {

}

void WATCardOffice::Courier::main() {
	printer->print( Printer::Kind::Courier, id, 'S' ); // courier starts

	for ( ; ; ) {
		_Accept( WATCardOffice::~Courier ) {
			break;
		} _Else {
			job = watCardOffice->requestWork(); // get a job from the office

			if ( job == NULL ) {
				break;
			}

			printer->print( Printer::Kind::Courier, id, 't', job->sid, job->amount ); // courier starts fund transfer

			watCardOffice->bank->withdraw( job->sid, job->amount ); // get money from the bank

			WATCard *watCard;
			if ( job->card == NULL ) { // student wants to create a new watcard
				watCard = new WATCard();
			} else { // student is updating current watcard
				watCard = job->card;
			}

			watCard->deposit( job->amount ); // deposit the retrieved money into the watcard

			int lost = mprng(1, 6); // randomly loses the watcard
			if ( lost == 1 ) { // watcard lost
				job->result.exception( new Lost );

				delete watCard;
			} else { // didn't lose watcard
				job->result.delivery( watCard );
			}

			printer->print( Printer::Kind::Courier, id, 'T', job->sid, job->amount ); // courier finished fund transfer

			delete job;
		}
	}

	printer->print( Printer::Kind::Courier, id, 'F' ); // courier is done
}