#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "Printer.h"
#include "NameServer.h"
#include "WATCardOffice.h"
#include "Groupoff.h"

_Task Student {
	Printer *printer;
	NameServer *nameServer;
	WATCardOffice *watCardOffice;
	Groupoff *groupoff;
	unsigned int id;
	unsigned int maxPurchases;

    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );
};

#endif