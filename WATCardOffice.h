#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

#include "Printer.h"
#include "Bank.h"
#include "WATCard.h"

#include <queue>

using namespace std;

_Task WATCardOffice {
    Printer *printer; // printer
    Bank *bank; // bank
    unsigned int numCouriers; // number of couriers
    uCondition noJob; // if there's no job, couriers will wait here

    struct Job {                           // marshalled arguments and return future
        // Args args;                         // call arguments (YOU DEFINE "Args")
        unsigned int sid;
        unsigned int amount;
        WATCard *card;
        WATCard::FWATCard result;          // return future

        Job( unsigned int sid, unsigned int amount, WATCard *card = NULL );
    };

    queue<Job*> jobs; // jobs waiting to be carried out

    _Task Courier {                  // communicates with bank
        private:
            WATCardOffice * watCardOffice; // to get job from
            Printer *printer;
            Bank *bank;
            Job *job; // this courier's job
            unsigned int id;

            void main();

        public:
            Courier( WATCardOffice *watCardOffice, Printer *printer, Bank *bank, unsigned int id );
    };

    Courier **couriers; // couriers to send requests to the bank

    void main();
  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif