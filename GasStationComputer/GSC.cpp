#include "GSC.h"
#include <ctime>
#include <string>
#include <sstream>
#include <stdio.h>

GSC::GSC() {

}

GSC::~GSC() {

}



bool dispense[NUMBER_OF_PUMPS];

void ReadKey()
{
	int command1;
	//int command2;
	command1 = _getch();
	_putch(command1);
	switch (command1) {
	case '1':	dispense[0] = true; break;
	case '2': 	dispense[1] = true; break;
	case '3': 	dispense[2] = true; break;
	case '4': 	dispense[3] = true; break;
	default: break;

	}


}

UINT __stdcall pump_user_status_thread(void *args) {

	int Thread_Number = *(int *)(args);
	string pump_name = "Pump";
	string pumpPS = "PS";
	string pumpCS = "CS";
	// create an ostringstream
	// assign each pump with a name
	std::ostringstream oss;
	oss << Thread_Number;
	
	pump_name = pump_name + oss.str();
	pumpPS = pumpPS + oss.str();
	pumpCS = pumpCS + oss.str();
	
	struct pumpInfo *pumpData;

	CDataPool *datapool =  new CDataPool (pump_name, sizeof(pumpInfo));
	pumpData = (struct pumpInfo *)(datapool->LinkDataPool());

	CSemaphore *PS = new CSemaphore (pumpPS, 0, 1);
	CSemaphore *CS = new CSemaphore (pumpCS, 1, 1);

	// GSC Command Semaphore
	CSemaphore *GSCPumpCost = new CSemaphore(pump_name + "GSCCommand", 0, 1);

	while (1) 
	{

		// check which producer is pushing the message first
		PS->Wait();
		printf("Pump %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
			pumpData->pumpID,
			MAX_NAME_LENGTH,
			pumpData->userName,
			pumpData->creditCard_1,
			pumpData->creditCard_2,
			pumpData->creditCard_3,
			pumpData->creditCard_4,
			pumpData->fuelType,
			pumpData->fuelAmount);


		// forever loop at here when GSC haven't authorise or reject it
		while (dispense[Thread_Number - 1] != true) {} 
		// when gas station attendant gives a command to pump
		if (dispense[Thread_Number - 1] = true)
		{
			pumpData->dispense_enable = 1;
			printf("pump%d is dispending fuel \n", Thread_Number);
			dispense[Thread_Number - 1] = false;
		}
		CS->Signal();



		// Up dated the Gas Computer how much oil has been dispensed and the cost
		while (GSCPumpCost->Read() == 0) // if the pump havn't finished dispending
		{
			PS->Wait();
			printf("dispensed Fuel is %.1f, and cost is %.1f  \n", pumpData->dispensedFuel, pumpData->cost);
			printf("GSCPumpCost->Read() is:  %d \n", GSCPumpCost->Read());
			printf("Customer %-*s  \n", MAX_NAME_LENGTH, pumpData->userName);
			CS->Signal();
			// Note: Here is an important break statement
			// Each time after cs->signal, we should check whether the fuelamount and dispensedfuel are equal or not
			// If they are equal, then we finished the fill up process, then we should break the while loop
			// other wise program will go back to ps->wait and give error output 
			if (pumpData->dispensedFuel == pumpData->fuelAmount)
			{
				break;
			}
		}

		printf("Customer %-*s is leaving the pump \n", MAX_NAME_LENGTH, pumpData->userName);
		GSCPumpCost->Wait();
		
	}

	return 0;
}



int GSC::main(void) {

	printf("Hello ------ GSC \n");

	int ThreadNum[NUMBER_OF_PUMPS] = { 1,2,3,4 };
	CThread *Datapool_Thread[NUMBER_OF_PUMPS]; 

	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		Datapool_Thread[i] = new CThread(pump_user_status_thread, SUSPENDED, &ThreadNum[i]);
		Datapool_Thread[i]->Resume();
	}

	
	// testing 
	while (1) {
		ReadKey();
	}

	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		Datapool_Thread[i]->WaitForThread();
	}
	
	return 0;
}