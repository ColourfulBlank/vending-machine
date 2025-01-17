#ifndef __PRINTER_H__
#define __PRINTER_H__

_Monitor Printer {
  private:
  	char *states;
  	int *value1;
  	int *value2;

  	unsigned int numObjects;
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;

  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

  private:
  	void flush( Kind kind, char state, unsigned int lid = 0 );
};

#endif