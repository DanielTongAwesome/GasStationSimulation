#ifndef __Pump__
#define __Pump__

#include <stdio.h>
#include "..\rt.h"
#include "..\GasStationSimulation\constants.h"

// pump class ---- ActiveClass
class Pump : public ActiveClass {

private:
	// common variable
	int pumpID;
	string pumpName;
	string pumpPS;
	string pumpCS;

	// initial data struct for pump
	struct pumpInfo *myPumpData;
	
	// pipeline
	CTypedPipe<struct customerInfo> *pipeline;

	// gate semaphore
	CSemaphore *EntryGate;
	CSemaphore *ExitGate;
	CSemaphore *Full;
	CSemaphore *Empty;

	// datapool
	CDataPool *pumpDatapool;

	// datapool semaphore
	CSemaphore *PS;
	CSemaphore *CS;

	// GSC Command Semaphore
	CSemaphore *GSCCommand;
	// GSC real time display dispense fuel and cost
	CSemaphore *GSCPumpCost;

public:

	// constructor
	Pump(int pump_ID);
	// destructor
	~Pump();
	

private:

	// running main
	int main(void);
};



#endif