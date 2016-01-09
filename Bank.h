#ifndef __BANK_H__
#define __BANK_H__

_Monitor Bank {
  private:
  	uCondition *insufficientFunds; // courier waits at its respective student account
  	unsigned int *studentFunds; // funds for each student

  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif