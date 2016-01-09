#ifndef __TRUCK_H__
#define __TRUCK_H__

#include "Printer.h"
#include "NameServer.h"
#include "BottlingPlant.h"

_Task Truck {
	Printer *printer;
	NameServer *nameServer;
	BottlingPlant *bottlingPlant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;
	unsigned int cargo[4]; // cargo containing the drinks to deliver to the vending machines

    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};
#endif