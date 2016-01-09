#include "Printer.h"
#include <iostream>

using namespace std;

static unsigned int numStaticObjects = 6;

// constructor
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ): numStudents( numStudents ), numVendingMachines( numVendingMachines ), numCouriers( numCouriers ) {
	numObjects = numStaticObjects + numStudents + numVendingMachines + numCouriers;
	states = new char[numObjects]; // array to store states
	value1 = new int[numObjects]; // array to store value1
	value2 = new int[numObjects]; // array to store value2

	// initialize the arrays
	for ( unsigned int i = 0; i < numObjects; i++ ) {
		states[i] = ' ';
		value1[i] = 0;
		value2[i] = 0;
	}

	// print header
	cout << "Parent" << '\t';
	cout << "Gropoff" << '\t';
	cout << "WATOff" << '\t';
	cout << "Names" << '\t';
	cout << "Truck" << '\t';
	cout << "Plant" << '\t';
	for ( unsigned int i = 0; i < numStudents; i++ ) {
		cout << "Stud" << i << '\t';
	}
	for ( unsigned int i = 0; i < numVendingMachines; i++ ) {
		cout << "Mach" << i << '\t';
	}
	for ( unsigned int i = 0; i < numCouriers; i++ ) {
		cout << "Cour" << i << '\t';
	}
	cout << endl;
	for ( unsigned int i = 0; i < numObjects; i++ ) {
		cout << "*******" << '\t';
	}
	cout << endl;
}

// clean up dynamically allocated arrays
Printer::~Printer() {
	cout << "***********************" << endl;

	delete [] states;
	delete [] value1;
	delete [] value2;
}

void Printer::flush( Kind kind, char state, unsigned int lid ) {
	unsigned int id;
	if ( ( unsigned int ) kind < 6 ) {
		id = ( unsigned int ) kind;
	} else if ( ( unsigned int ) kind == 6 ) {
		id = ( unsigned int ) kind + lid;
	} else if ( ( unsigned int ) kind == 7 ) {
		id = 6 + numStudents + lid;
	} else {
		id = 6 + numStudents + numVendingMachines + lid;
	}

	bool allEmpty = true;
	if ( state == 'F' ) {
		for ( unsigned int i = 0; i < numObjects; i++ ) {
			if ( states[i] != ' ' ) {
				allEmpty = false;

				break;
			}
		}
	} else if ( states[id] == ' ' ) {
		return;
	}
	

	if ( !allEmpty || state != 'F' ) {
		for ( unsigned int i = 0; i < numObjects; i++ ) {
			Kind k;
			if ( i < 6 ) {
				k = ( Kind ) i;
			} else if ( i < 6 + numStudents ) {
				k = Kind::Student;
			} else if ( i < 6 + numStudents + numVendingMachines ) {
				k = Kind::Vending;
			} else {
				k = Kind::Courier;
			}

			switch( states[i] ) {
				case 'S': // start
					cout << "S";
					if ( k == Kind::Student ) {
						cout << value1[i] << "," << value2[i];
					} else if ( k == Kind::Vending ) {
						cout << value1[i];
					}
					cout << '\t';
					
					break;
				case 'D': // deposit gift
// cout << endl << "kind: " << kind << " state: " << state << endl;
					if ( k == Kind::Groupoff ) { // groupoff
// cout << endl << "afdasdfasdf " << i << endl;
						cout << "D" << value1[i] << '\t';
					} else { // parent or truck
						cout << "D" << value1[i] << "," << value2[i] << '\t';
					}
					
					break;
				case 'W': // request work call complete
					cout << "W" << '\t';

					break;
				case 'C': // create call complete
					cout << "C" << value1[i] << "," << value2[i] << '\t';

					break;
				case 'T': // transfer call complete
					cout << "T" << value1[i] << "," << value2[i] << '\t';

					break;
				case 't': // transfer call complete
					cout << "t" << value1[i] << "," << value2[i] << '\t';

					break;
				case 'R': // register vending machine
					if ( k == Kind::NameServer ) { // name server
						cout << "R" << value1[i] << '\t';
					} else { // vending machine
						cout << "R" << '\t';
					}

					break;
				case 'N': // new vending machine
					cout << "N" << value1[i] << "," << value2[i] << '\t';

					break;
				case 'P': // pick up shipment
					if ( k == Kind::Truck ) { // truck
						cout << "P" << value1[i] << '\t';
					} else { // plant
						cout << "P" << '\t';
					}

					break;
				case 'd': // begin delivery to vending machine
					cout << "d" << value1[i] << "," << value2[i] << '\t';

					break;
				case 'U': // unsuccessfully filled vending machine
					cout << "U" << value1[i] << "," << value2[i] << '\t';

					break;
				case 'V': // unsuccessfully filled vending machine
					cout << "V" << value1[i] << '\t';

					break;
				case 'B': // unsuccessfully filled vending machine
					cout << "B" << value1[i];
					if ( Kind::Vending == k ) {
						cout << "," << value2[i];
					}
					cout << '\t';

					break;
				case 'G': // unsuccessfully filled vending machine
					cout << "G" << value1[i] << '\t';

					break;
				case 'L': // unsuccessfully filled vending machine
					cout << "L" << '\t';

					break;
				case 'r': // unsuccessfully filled vending machine
					cout << "r" << '\t';

					break;
				default:
					cout << '\t';
			}

			states[i] = ' '; // reset the state of this object
		}
		cout << endl;
	}
	
// cout << "Kind: " << kind << " state: " << state << endl;
	if ( state == 'F' ) { // this object is finished
		for ( unsigned int i = 0; i < numObjects; i++ ) {
			if ( i != id ) {
				cout << "..." << '\t';
			} else {
				cout << 'F' << '\t';
			}
		}
		cout << endl;
	}
}

// print start state
void Printer::print( Kind kind, char state ) {
	flush( kind, state ); // flush objects
// cout << "Kind: " << kind << " state: " << state << endl;
	if ( state != 'F' ) { // only add new state if it's not finished
		states[kind] = state; // new state
	}
}

void Printer::print( Kind kind, char state, int value1 ) {
	flush( kind, state ); // flush objects

	states[kind] = state; // new state
	Printer::value1[kind] = value1; // new value1
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
	flush( kind, state ); // flush objects

	states[kind] = state; // new state
	Printer::value1[kind] = value1; // new value1
	Printer::value2[kind] = value2; // new value2
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
	flush( kind, state, lid ); // flush objects

	if ( state != 'F' ) { // only add new state if it's not finished
		unsigned int id;
		if ( ( unsigned int ) kind < 6 ) {
			id = ( unsigned int ) kind;
		} else if ( ( unsigned int ) kind == 6 ) {
			id = ( unsigned int ) kind + lid;
		} else if ( ( unsigned int ) kind == 7 ) {
			id = 6 + numStudents + lid;
		} else {
			id = 6 + numStudents + numVendingMachines + lid;
		}
		states[id] = state; // new state
	}
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
	flush( kind, state, lid ); // flush objects

	unsigned int id;
	if ( ( unsigned int ) kind < 6 ) {
		id = ( unsigned int ) kind;
	} else if ( ( unsigned int ) kind == 6 ) {
		id = ( unsigned int ) kind + lid;
	} else if ( ( unsigned int ) kind == 7 ) {
		id = 6 + numStudents + lid;
	} else {
		id = 6 + numStudents + numVendingMachines + lid;
	}

	states[id] = state; // new state
	Printer::value1[id] = value1; // new value1
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
	flush( kind, state, lid ); // flush objects

	unsigned int id;
	if ( ( unsigned int ) kind < 6 ) {
		id = ( unsigned int ) kind;
	} else if ( ( unsigned int ) kind == 6 ) {
		id = ( unsigned int ) kind + lid;
	} else if ( ( unsigned int ) kind == 7 ) {
		id = 6 + numStudents + lid;
	} else {
		id = 6 + numStudents + numVendingMachines + lid;
	}

	states[id] = state; // new state
	Printer::value1[id] = value1; // new value1
	Printer::value2[id] = value2; // new value2
}