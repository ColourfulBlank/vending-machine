#include "Parent.h"
#include "Printer.h"
#include "Bank.h"
#include "MPRNG.h"

void Parent::main() {
	printer->print( Printer::Kind::Parent, 'S' ); // parent is starting

	for ( ; ; ) {
		_Accept( ~Parent ) {
			printer->print( Printer::Kind::Parent, 'F' ); // parent is finished

			break;
		} _Else {
			unsigned int gift = mprng( 1, 3 ); // randomize the amount of money to give to the student
			unsigned int id = mprng( numStudents - 1 ); // randomize the student to give the money to

			yield( parentalDelay ); // parent yields before gift

			bank->deposit( id, gift ); // deposit the money to the specific student
			printer->print( Printer::Kind::Parent, 'D', id, gift );
		}
	}
}

// constructor
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ): printer( &prt ), bank( &bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {}