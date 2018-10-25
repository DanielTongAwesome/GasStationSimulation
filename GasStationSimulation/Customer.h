#ifndef __Customer__
#define __Customer__

#include <stdio.h>
#include "..\rt.h"
#include "constants.h"

// class for customer --- ActiveClass
class Customer : public ActiveClass {
private:
	int destination_pump_number;
	struct customerInfo customerData;
	CTypedPipe<struct customerInfo> *pipeline;
	CSemaphore *EntryGate;
	CSemaphore *ExitGate;
	CSemaphore *Full;
	CSemaphore *Empty;


	
public:
	// constructor
	Customer(int go_to_pump);
	// destructor 
	~Customer(void);
	// running main
	int main(void);
};

#endif