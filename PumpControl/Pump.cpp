#include "Pump.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <ctime>

Pump::Pump(int pump_ID)
{
	// assign each pump with an ID
	pumpID = pump_ID;
	// create an ostringstream
	// assign each pump with a name
	std::ostringstream oss;
	oss << pumpID;
	pumpName = "Pump" + oss.str();

	// set up pipline to talk with pump and mutex
	pipeline = new CTypedPipe<struct customerInfo>(pumpName, 1);
	/*pipelineMutex = new CMutex(pumpName);
	pumpName = "Pump2" + oss.str();
	pipelineMutex2 = new CMutex(pumpName);*/
	EntryGate = new CSemaphore(pumpName + "EntryGate", 0, 1);
	ExitGate = new CSemaphore(pumpName + "ExitGate", 0, 1);
	Full = new CSemaphore(pumpName + "Full", 0, 1);
	Empty = new CSemaphore(pumpName + "Empty", 0, 1);

}

Pump::~Pump()
{
	/*delete pipelineMutex;
	delete pipelineMutex2;*/
	delete EntryGate;
	delete ExitGate;
	delete Full;
	delete Empty;

}

int Pump::main(void)
{
	struct customerInfo currentCustomer;
	
	while (1) {
	
		// traffic logic: control the entering of the customer
		EntryGate->Signal();
		Full->Wait();

		// read from the pipeline
		pipeline->Read(&currentCustomer);
		
	

		printf("Pipeline %d Current user: %s  CreditCard: %d  FuelType: %d  Amount: %d \n",
			pumpID,
			currentCustomer.name,
			currentCustomer.creditCard,
			currentCustomer.fuelType,
			currentCustomer.fuelAmount);
		SLEEP(1000);
		
		// traffic logic: finshed pumping and exict from the waitlist
		ExitGate->Signal();
		Empty->Wait();
	}
	return 0;
}
