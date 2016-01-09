#include "Truck.h"
#include "Printer.h"
#include "NameServer.h"
#include "BottlingPlant.h"
#include "VendingMachine.h"
#include "MPRNG.h"

#include <iostream>
using namespace std;

extern MPRNG mprng;

void Truck::main() {
	unsigned int currentMachine = 0; // current machine to start stocking
	unsigned int currentStartingMachine = 0; // starting machine for the current stock round

	printer->print( Printer::Kind::Truck, 'S' ); // truck starts

	for ( ; ; ) {
// cout << "truck getting machine list" << endl;		
		VendingMachine **machineList = nameServer->getMachineList(); // get the machine list from name server
// cout << "TM go" << endl;
		yield( mprng( 1, 10 ) ); // yields random number of time
// cout << "TM back" << endl;
		try {
			bottlingPlant->getShipment( cargo ); // get shipment from the plant
// cout << cargo[0] << " " << cargo[1] << " " << cargo[2] << " " << cargo[3] << endl;
		} catch ( BottlingPlant::Shutdown shutDown ) { // plant has closed down
// cout << "truck got shutdown" << endl;
			delete [] machineList;

			break;
		}

		// count amount of sodas in shipment
		unsigned int count = 0;
		for ( int i = 0; i < 4; i++ ) {
			count += cargo[i];
		}

		printer->print( Printer::Kind::Truck, 'P', count ); // truck picked up shipment from bottling plant

		currentStartingMachine = currentMachine; // the starting machine of this round of loads
		for ( ; ; ) {
			printer->print( Printer::Kind::Truck, 'd', machineList[currentMachine]->getId(), count ); // truck begins delivery to vending machines

			unsigned int totalUnfilled = 0;

			unsigned int *inv = machineList[currentMachine]->inventory();
			for ( unsigned int i = 0; i < 4; i++ ) {
				if ( inv[i] + cargo[i] <= maxStockPerFlavour ) { // can load all of cargo
					inv[i] += cargo[i];

					count -= cargo[i];

					cargo[i] = 0;

					totalUnfilled += maxStockPerFlavour - inv[i];
				} else { // cannot load all of cargo
					cargo[i] -= maxStockPerFlavour - inv[i];

					count -= maxStockPerFlavour - inv[i];

					inv[i] = maxStockPerFlavour;
				}
			}
			machineList[currentMachine]->restocked(); // signal restocked

			// check for unsuccessfull filling
			if ( totalUnfilled != 0 ) {
				printer->print( Printer::Kind::Truck, 'U', machineList[currentMachine]->getId(), totalUnfilled ); // unsuccessful filling
			}

			printer->print( Printer::Kind::Truck, 'D', machineList[currentMachine]->getId(), count ); // unsuccessful filling

			currentMachine = ( currentMachine + 1 ) % numVendingMachines;
			
			if ( cargo[0] == 0 && cargo[1] == 0 && cargo[2] == 0 && cargo[3] == 0 ) break;
			if ( currentMachine == currentStartingMachine ) break;
		}
// cout << "delete list" << endl;
		delete [] machineList;
// cout << "ded list" << endl;
	}
// cout << "truck about to print finish" << endl;
	printer->print( Printer::Kind::Truck, 'F' ); // truck ends
}

// constructor
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ): printer( &prt ), nameServer( &nameServer ), bottlingPlant( &plant ), numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour ) {
	for ( int i = 0; i < 4; i++ ) { // cargo starts out empty
		cargo[i] = 0;
	}
}