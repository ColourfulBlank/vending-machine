#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include "Printer.h"
#include "NameServer.h"
#include "WATCard.h"

_Task NameServer;

_Task VendingMachine {
    Printer *printer;
    NameServer *nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;
    unsigned int currentStock[4]; // current stock for all flavours

    void main();
  public:
    enum Flavours { You, Only, Live, Once };                 // flavours of soda (YOU DEFINE)
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    ~VendingMachine();
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();

  private:
    unsigned int fundsStock;
    uCondition checkFundsStock;
    Flavours currentFlavour;
    WATCard *currentWATCard;
};

#endif