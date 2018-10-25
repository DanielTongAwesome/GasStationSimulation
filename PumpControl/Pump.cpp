#include "Pump.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <ctime>


// pump constructor
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

	// create semaphores 
	EntryGate = new CSemaphore(pumpName + "EntryGate", 0, 1);
	ExitGate = new CSemaphore(pumpName + "ExitGate", 0, 1);
	Full = new CSemaphore(pumpName + "Full", 0, 1);
	Empty = new CSemaphore(pumpName + "Empty", 0, 1);

}


// pump destructor
Pump::~Pump()
{
	// delete the created pointer to release memeory
	delete EntryGate;
	delete ExitGate;
	delete Full;
	delete Empty;

}


// pump main 
int Pump::main(void)
{
	// craete the data struct to store the info send through pipeline
	struct customerInfo currentCustomer;
	
	// each pump runs the following logic
	while (1) {
	
		// traffic logic: control the entering of the customer
		EntryGate->Signal();
		Full->Wait();

		// read from the pipeline
		pipeline->Read(&currentCustomer);
		
		printf("Pipeline %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
			pumpID,
			MAX_NAME_LENGTH,
			currentCustomer.name,
			currentCustomer.creditCard_1,
			currentCustomer.creditCard_2,
			currentCustomer.creditCard_3,
			currentCustomer.creditCard_4,
			currentCustomer.fuelType,
			currentCustomer.fuelAmount);
		




		SLEEP(1000);
		
		// traffic logic: finshed pumping and exict from the waitlist
		ExitGate->Signal();
		Empty->Wait();
	}
	return 0;
}
