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

	//create a screen control mutex
	screenMutex = new CMutex("PumpScreen");
	myPipeMutex = new CMutex("PumpName");

	// create datapool for PUMP and GSC
	pumpDatapool = new CDataPool(pumpName, sizeof(pumpInfo));
	myPumpData = (struct pumpInfo *)(pumpDatapool->LinkDataPool());

	// datapool semaphores
	PS = new CSemaphore(pumpPS, 0, 1);
	CS = new CSemaphore(pumpCS, 1, 1);

	// GSC Command Semaphore
	GSCCommand = new CSemaphore(pumpName + "GSCCommand", 0, 1);
	GSCPumpCost = new CSemaphore(pumpName + "GSCCommand", 0, 1);

	
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

// pump clearline function cr empty lineseate 
void Pump::clearLine(int lineNum)
{
	screenMutex->Wait();
	MOVE_CURSOR(0, lineNum);
	printf("                       "
		"                           \n");
	screenMutex->Signal();
}

// pump main 
int Pump::main(void)
{
	// craete the data struct to store the info send through pipeline
	struct customerInfo currentCustomer;
	int colourIndex = 15;

	// each pump runs the following logic
	while (1) {
	

		// traffic logic: control the entering of the customer
		EntryGate->Signal();
		Full->Wait();

		// read from the pipeline
		pipeline->Read(&currentCustomer);



		// head of the pump display monitor
		screenMutex->Wait();
		//                  ----> (  x   ,         y        )    
		// move the cursor to the (  0   ,  (pumpID - 1) * 8)
		MOVE_CURSOR(0, (pumpID - 1) * 8);
		colourIndex = pumpID + 9;
		// assign each punch with different color
		TEXT_COLOUR(colourIndex, 0);
		printf("======================================================\n");
		printf("---------------		Pump %d		--------------\n", pumpID);
		printf("======================================================\n");

		fflush(stdout);
		screenMutex->Signal();

		screenMutex->Wait();
		//                  ----> (  x   ,         y        )    
		// move the cursor to the (  0   ,  (pumpID - 1) * 8 + 3)
		MOVE_CURSOR(0, (pumpID - 1) * 8 + 3);
		colourIndex = pumpID + 9;
		TEXT_COLOUR(colourIndex, 0);
		// print currentUser infomation
		printf("Pump %d: Current user: %-*s \nCreditCard: %d %d %d %d  \nFuelType: %d  Amount: %d \n",
			pumpID,
			MAX_NAME_LENGTH,
			currentCustomer.name,
			currentCustomer.creditCard_1,
			currentCustomer.creditCard_2,
			currentCustomer.creditCard_3,
			currentCustomer.creditCard_4,
			currentCustomer.fuelType,
			currentCustomer.fuelAmount);
		fflush(stdout);
		screenMutex->Signal();
		TEXT_COLOUR(15, 0);


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
		myPumpData->dispensedFuel = 0;
		myPumpData->cost = 0;
		PS->Signal(); 


		// move cursor to (0, ((myPumpData->pumpID) - 1) * 8 + 6)
		screenMutex->Wait();
		TEXT_COLOUR(colourIndex, 0);
		MOVE_CURSOR(0, ((myPumpData->pumpID) - 1) * 8 + 6);
		printf("Customer is waiting for dispense......");
		fflush(stdout);
		screenMutex->Signal();
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
				screenMutex->Wait();
				TEXT_COLOUR(colourIndex, 0);
				// move cursor to (0, ((myPumpData->pumpID) - 1) * 8 + 6)
				MOVE_CURSOR(0, ((myPumpData->pumpID) - 1) * 8 + 6);
				printf("dispensed amount: %.1f cost: %.2f          \n", myPumpData->dispensedFuel, myPumpData->cost);			
				fflush(stdout);
				screenMutex->Signal();		
				PS->Signal();	
				SLEEP(100);
			}
			SLEEP(1000);
			GSCPumpCost->Signal();		
		}


		// if detects reject been enabled 
		else if (myPumpData->reject_enable == 1)
		{
			PS->Signal();
			// move cursor to ( 0 , (myPumpData->pumpID - 1) * 8 + 6 )
			screenMutex->Wait();
			MOVE_CURSOR(0, (myPumpData->pumpID - 1) * 8 + 6);
			TEXT_COLOUR(colourIndex, 0);
			// print reject infomation
			printf("pump%d rejects customer to fuel                               \n", myPumpData->pumpID);
			SLEEP(100);	
			MOVE_CURSOR(0, (myPumpData->pumpID - 1) * 8 + 6);
			printf("                                                ");
			screenMutex->Signal();
			fflush(stdout);
		}


		
		GSCCommand->Signal();


		// move cursor to ( 0 , (myPumpData->pumpID - 1) * 8 + 6 )
		SLEEP(1000);
		colourIndex = myPumpData->pumpID + 9;
		screenMutex->Wait();
		MOVE_CURSOR(0, (myPumpData->pumpID - 1) * 8 + 6);
		TEXT_COLOUR(colourIndex, 0);
		printf("Customer %-*s is leaving the pump                            \n", MAX_NAME_LENGTH, currentCustomer.name);
		// traffic logic: finshed pumping and exict from the waitlist
		Sleep(1000);
		MOVE_CURSOR(0, (myPumpData->pumpID - 1) * 8 + 6);
		printf("                                                ");
		screenMutex->Signal();
		fflush(stdout);
		ExitGate->Signal();
		Empty->Wait();
	}
	return 0;
}
