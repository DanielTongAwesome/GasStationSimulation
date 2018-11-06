#include "Pump.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <ctime>

// add one line in pump 
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
	pumpPS = "PS" + oss.str();
	pumpCS = "CS" + oss.str();

	// set up pipline to talk with pump and mutex
	pipeline = new CTypedPipe<struct customerInfo>(pumpName, 1);

	// create semaphores 
	EntryGate = new CSemaphore(pumpName + "EntryGate", 0, 1);
	ExitGate = new CSemaphore(pumpName + "ExitGate", 0, 1);
	Full = new CSemaphore(pumpName + "Full", 0, 1);
	Empty = new CSemaphore(pumpName + "Empty", 0, 1);

	// create datapool for PUMP and GSC
	pumpDatapool = new CDataPool(pumpName, sizeof(pumpInfo));
	myPumpData = (struct pumpInfo *)(pumpDatapool->LinkDataPool());

	// datapool semaphores
	PS = new CSemaphore(pumpPS, 0, 1);
	CS = new CSemaphore(pumpCS, 1, 1);

	// GSC Command Semaphore
	GSCCommand = new CSemaphore(pumpName + "GSCCommand", 0, 1);
	// 
	GSCPumpCost = new CSemaphore(pumpName + "GSCCommand", 0, 1);

	// create datapool for fueltank and Pump
	//tankDataPool = new CDataPool("FuelTankDataPool", sizeof(struct fuelTankInfo));
	//tank = (struct fuelTankInfo *)(tankDataPool->LinkDataPool());

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


		// save the current Customer's information into datapool
		CS->Wait();
		myPumpData->pumpID = pumpID;
		strcpy_s(myPumpData->userName, currentCustomer.name);
		myPumpData->creditCard_1 = currentCustomer.creditCard_1;
		myPumpData->creditCard_2 = currentCustomer.creditCard_2;
		myPumpData->creditCard_3 = currentCustomer.creditCard_3;
		myPumpData->creditCard_4 = currentCustomer.creditCard_4;
		myPumpData->fuelType = currentCustomer.fuelType;
		myPumpData->fuelAmount = currentCustomer.fuelAmount;
		myPumpData->SelectedFuelPrice = currentCustomer.SelectedFuelPrice;
		myPumpData->purchaseTime = currentCustomer.purchaseTime;
		myPumpData->reject_enable = 0;
		myPumpData->dispense_enable = 0;
		// when each customer comes to the pump
		// reset the dispensed fuel 
		// reset the cost
		//myPumpData->SelectedFuelPrice = 0;
		myPumpData->dispensedFuel = 0;
		myPumpData->cost = 0;
		PS->Signal(); 


		// when GSC has read data from datapool and 
		// receive authorisation from GSC via the datapool
		CS->Wait();   // Wait here until it receive a command from GSC
		
		
		// if detects dispense been enabled 
		if (myPumpData->dispense_enable == 1)
		{
			PS->Signal();
			while (float(myPumpData->fuelAmount) - myPumpData->dispensedFuel >= 0.5)
			{
				CS->Wait();
				// update the dispensed fuel to DOS
				myPumpData->dispensedFuel += PUMP_RATE;	//add dispensed Fuel		
				// update cost to the DOS
				myPumpData->cost = myPumpData->dispensedFuel * myPumpData->SelectedFuelPrice; // calculated cost
				// Display real time dispensed fuel and Cost on the pump 
				printf("pump%d    dispensed %.1f amount of fuel    cost %.1f  \n", myPumpData->pumpID, myPumpData->dispensedFuel, myPumpData->cost);			
				//printf(" %.1f \n", float(myPumpData->fuelAmount) - myPumpData->dispensedFuel);
				
				PS->Signal();	
				SLEEP(100);
			}
			GSCPumpCost->Signal();		
		}

		// if detects reject been enabled 
		else if (myPumpData->reject_enable == 1)
		{
			PS->Signal();
			printf("pump%d rejects customer to fuel \n", myPumpData->pumpID);
			SLEEP(100);		
		}


		
		GSCCommand->Signal();



		SLEEP(1000);
		printf("Customer %-*s is leaving the pump \n", MAX_NAME_LENGTH, currentCustomer.name);
		// traffic logic: finshed pumping and exict from the waitlist
		ExitGate->Signal();
		Empty->Wait();
	}
	return 0;
}
