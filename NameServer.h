#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include "Printer.h"
#include "VendingMachine.h"

#include <vector>

_Task VendingMachine;

using namespace std;

_Task NameServer {
	Printer *printer;
	unsigned int numVendingMachines;
	unsigned int numStudents;
	vector<VendingMachine*> vendingMachines; // vending machines
	unsigned int *nextMachines; // which machine each student will get next

    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif