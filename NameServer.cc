#include "NameServer.h"
#include "VendingMachine.h"

#include <vector>
#include <iostream>

using namespace std;

void NameServer::main() {
	printer->print( Printer::Kind::NameServer, 'S' ); // name server starts

	for ( ; ; ) {
		_Accept( ~NameServer ) {
			break;
		} or _Accept( VMregister ) { // for vending machines

		} or _Accept( getMachineList ) { // for trucks

		} or _Accept( getMachine ) { // for students

		}
	}

	printer->print( Printer::Kind::NameServer, 'F' ); // name server ends
}

// constructor
NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ): printer( &prt ), numVendingMachines( numVendingMachines ), numStudents( numStudents ) {
	nextMachines = new unsigned int[numStudents];
	for ( unsigned int i = 0; i < numStudents; i++ ) {
		nextMachines[i] = 0;
	}
}

// clean up dynamically allocated arrays
NameServer::~NameServer() {
	delete [] nextMachines;
}

// vending machine registers itself
void NameServer::VMregister( VendingMachine *vendingmachine ) {
	vendingMachines.push_back( vendingmachine );

	printer->print( Printer::Kind::NameServer, 'R', vendingmachine->getId() );
}

// student trying to get a machine
VendingMachine *NameServer::getMachine( unsigned int id ) {
	unsigned int index = ( nextMachines[id] + id ) % vendingMachines.size(); // index of the vending machine this student will get
// cout << "index: " << index << " nextMachines[id]: " << nextMachines[id] << " id: " << id << " vendingmachines size: " << vendingMachines.size() << endl;
	nextMachines[id]++; // this student will get another machine next time
// cout << "vending machine's id: " << vendingMachines[index]->getId() << endl;
	printer->print( Printer::Kind::NameServer, 'N', id, vendingMachines[index]->getId() );

	return vendingMachines[index];
}

// truck will get all of the vending machines
VendingMachine **NameServer::getMachineList() {
// cout << "just got into machine list" << endl;
	unsigned int length = vendingMachines.size(); // current number of registered vending machines

	// convert the vector into **
	VendingMachine **ret = new VendingMachine*[length];
	for ( unsigned int i = 0; i < length; i++ ) {
		ret[i] = vendingMachines[i];
	}
// cout << "about to return machine list" << endl;
	return ret;
}