#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

#include "Printer.h"
#include "WATCard.h"

#include <vector>

using namespace std;

_Task Groupoff {
	Printer *printer;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	vector<WATCard::FWATCard> giftCards;
	vector<int> giftCardAvailable;

    void main();

  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif