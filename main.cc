#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "MPRNG.h"
#include "config.h"
#include "Printer.h"
#include "Bank.h"
#include "Parent.h"
#include "WATCardOffice.h"
#include "Groupoff.h"
#include "NameServer.h"
#include "VendingMachine.h"
#include "BottlingPlant.h"
#include "Student.h"

using namespace std;

MPRNG mprng;

// print error usage
static void printUsage() {
	cout << "Usage: ./soda [ config-file [ random-seed (> 0) ] ]" << endl;
}

// check seed value
static bool checkSeed( char *c ) {
	istringstream iss( c );
	int temp;
	iss >> temp;
	if ( iss.fail() || temp < 0 ) { // invalid seed
		return false;
	}

	return true;
}

void uMain::main() {
	string fileName = "soda.config"; // default file name
	unsigned int seed = getpid(); // default seed

	struct ConfigParms cParms; // config parameters

	switch ( argc ) {
		case 3: // config file + seed
			// parse the seed argument
			if ( !checkSeed( argv[2] ) ) { // invalid seed
				printUsage();

				return;
			}
			seed = atoi( argv[2] );

		case 2: // config file only
			// get the custom file name
			fileName = argv[1];

		case 1: // no config and no seed
			break;

		default: // all other forms of commandline arguments are invalid
			printUsage();

			return;
	}

	mprng.seed( seed ); // seed the rng

	// process the config file
	processConfigFile( fileName.c_str(), cParms );

	Printer printer( cParms.numStudents, cParms.numVendingMachines, cParms.numCouriers );

	Bank bank( cParms.numStudents );

	Parent parent( printer, bank, cParms.numStudents, cParms.parentalDelay );

	WATCardOffice watCardOffice( printer, bank, cParms.numCouriers );

	Groupoff groupoff( printer, cParms.numStudents, cParms.sodaCost, cParms.groupoffDelay );

	NameServer nameServer( printer, cParms.numVendingMachines, cParms.numStudents );

	VendingMachine *vendingMachines[cParms.numVendingMachines];
	for ( unsigned int i = 0; i < cParms.numVendingMachines; i++ ) {
		vendingMachines[i] = new VendingMachine( printer, nameServer, i, cParms.sodaCost, cParms.maxStockPerFlavour );
	}

	BottlingPlant *bottlingPlant = new BottlingPlant( printer, nameServer, cParms.numVendingMachines, cParms.maxShippedPerFlavour, cParms.maxStockPerFlavour, cParms.timeBetweenShipments );

	Student *students[cParms.numStudents];
	for ( unsigned int i = 0; i < cParms.numStudents; i++ ) {
		students[i] = new Student( printer, nameServer, watCardOffice, groupoff, i, cParms.maxPurchases );
	}

	// synchronize the end of students, plant, and vending machines
	/////

	for ( unsigned int i = 0; i < cParms.numStudents; i++ ) {
		delete students[i];
	}

	delete bottlingPlant;

	for ( unsigned int i = 0; i < cParms.numVendingMachines; i++ ) {
		delete vendingMachines[i];
	}
}