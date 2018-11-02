#ifndef __Customer__
#define __Customer__

#include <stdio.h>
#include "..\rt.h"
#include "constants.h"

// class for customer --- ActiveClass
class Customer : public ActiveClass {

private:
	// common variable
	int destination_pump_number;

	// initial data struct for customer
	struct customerInfo customerData;

	// pipeline 
	CTypedPipe<struct customerInfo> *pipeline;
	
	// gate semaphore
	CSemaphore *EntryGate;
	CSemaphore *ExitGate;
	CSemaphore *Full;
	CSemaphore *Empty;

	// GSC Command Semaphore
	CSemaphore *GSCCommand;
	
public:
	// constructor
	Customer(int go_to_pump);
	// destructor 
	~Customer(void);


private:
	// running main
	int main(void);
};

#endif