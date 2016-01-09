#include "Bank.h"

// constructor
Bank::Bank( unsigned int numStudents ) {
	insufficientFunds = new uCondition[numStudents];
	studentFunds = new unsigned int[numStudents];

	// students start with $0
	for ( unsigned int i = 0; i < numStudents; i++ ) {
		studentFunds[i] = 0;
	}
}

// clean up dynamically allocated arrays
Bank::~Bank() {
	delete [] insufficientFunds;
	delete [] studentFunds;
}

// parent deposit money to a student
void Bank::deposit( unsigned int id, unsigned int amount ) {
	studentFunds[id] += amount; // put the money into the student's account

	// as long as the next courier has sufficient funds to take away
	while( !insufficientFunds[id].empty() && insufficientFunds[id].front() <= studentFunds[id] ){
		insufficientFunds[id].signal(); // signal the courier

		studentFunds[id] -= amount; // decrease the funds from the student's account
	}
	
}

// courier withdraw funds from a student's account
void Bank::withdraw( unsigned int id, unsigned int amount ) {
	if ( studentFunds[id] < amount ) { // not enough money in the student's account
		insufficientFunds[id].wait( amount );
	} else {
		studentFunds[id] -= amount;
	}
}