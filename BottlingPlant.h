#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

#include "Printer.h"
#include "NameServer.h"

_Task BottlingPlant {
	Printer *printer;
	NameServer *nameServer;
	unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
	unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
	unsigned int products[4]; // products to go onto cargo of truck
	bool closingDown;

    void main();
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};

#endif