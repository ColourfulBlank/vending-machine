#include "BottlingPlant.h"
#include "Printer.h"
#include "NameServer.h"
#include "Truck.h"
#include "MPRNG.h"

#include <iostream>
using namespace std;

extern MPRNG mprng;

void BottlingPlant::main() {
	printer->print( Printer::Kind::BottlingPlant, 'S' ); // bottling plant is starting

	Truck truck( *printer, *nameServer, ( BottlingPlant & ) uThisTask(), numVendingMachines, maxStockPerFlavour ); // the truck to transport

	for ( ; ; ) {
		try {
			yield( timeBetweenShipments ); // yield before shipments

			int bottles = 0;
			for ( int i = 0; i < 4; i++ ) { // produces drinks
				products[i] = mprng( 0, maxShippedPerFlavour );

				bottles += products[i];
			}

			printer->print( Printer::Kind::BottlingPlant, 'G', bottles ); // bottling plant is generating bottles

			_Accept( ~BottlingPlant ) {
// cout << "bottling plant just got called delete" << endl;
				closingDown = true;

				_Accept( getShipment ) {

				}

				break;
			} or _Accept( getShipment ) {

			}
		} catch ( uMutexFailure::RendezvousFailure ) {
			break;
		}
	}

	printer->print( Printer::Kind::BottlingPlant, 'F' ); // bottling plant is ending
}

// constructor
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ): printer( &prt ), nameServer( &nameServer ), numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments ) {
	closingDown = false;
}

// destructor
BottlingPlant::~BottlingPlant() {

}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
	if ( closingDown ) {
		_Throw Shutdown();
	}

	for ( int i = 0; i < 4; i++ ) { // put the productions onto cargo
		cargo[i] = products[i];
	}

	printer->print( Printer::Kind::BottlingPlant, 'P' ); // shipment picked up by truck
}