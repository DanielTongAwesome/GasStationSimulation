#include "GSC.h"
#include <ctime>
#include <string>
#include <sstream>
#include <stdio.h>

GSC::GSC() {

	//// inital 8 semaphores for 4 datapools
	//PS1 = new CSemaphore(pumpPS_1, 0, 1);
	//CS1 = new CSemaphore(pumpCS_1, 1, 1);
	//PS2 = new CSemaphore(pumpPS_2, 0, 1);
	//CS2 = new CSemaphore(pumpCS_2, 1, 1);
	//PS3 = new CSemaphore(pumpPS_3, 0, 1);
	//CS3 = new CSemaphore(pumpCS_3, 1, 1);
	//PS4 = new CSemaphore(pumpPS_4, 0, 1);
	//CS4 = new CSemaphore(pumpCS_4, 1, 1);

	//// initial 4 datapools
	//datapool1 = new CDataPool(pumpName_1, sizeof(pumpInfo));
	//datapool2 = new CDataPool(pumpName_2, sizeof(pumpInfo));
	//datapool3 = new CDataPool(pumpName_3, sizeof(pumpInfo));
	//datapool4 = new CDataPool(pumpName_4, sizeof(pumpInfo));
	//PumpData_1 = (struct pumpInfo *)(datapool1->LinkDataPool());
	//PumpData_2 = (struct pumpInfo *)(datapool2->LinkDataPool());
	//PumpData_3 = (struct pumpInfo *)(datapool3->LinkDataPool());
	//PumpData_4 = (struct pumpInfo *)(datapool4->LinkDataPool());
}

GSC::~GSC() {
	//delete PumpData_1;
	//delete PumpData_2;
	//delete PumpData_3;
	//delete PumpData_4;
	//delete datapool1;
	//delete datapool2;
	//delete datapool3;
	//delete datapool4;
	//delete PS1;
	//delete PS2;
	//delete PS3;
	//delete PS4;
	//delete CS1;
	//delete CS2;
	//delete CS3;
	//delete CS4;
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

	while (1) {
		// check which producer is pushing the message first
		if (PS->Read() > 0) {
			
			PS->Wait();

			printf("Pipeline %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
				pumpData->pumpID,
				MAX_NAME_LENGTH,
				pumpData->userName,
				pumpData->creditCard_1,
				pumpData->creditCard_2,
				pumpData->creditCard_3,
				pumpData->creditCard_4,
				pumpData->fuelType,
				pumpData->fuelAmount);

			CS->Signal();
		}
		else {
			continue;
		}
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

	
	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		Datapool_Thread[i]->WaitForThread();
	}
	
	//while (TRUE) {
	//	// check which producer is pushing the message first
	//	if (PS1->Read() > 0) {

	//		PS1->Wait();

	//		printf("Pipeline %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
	//			PumpData_1->pumpID,
	//			MAX_NAME_LENGTH,
	//			PumpData_1->userName,
	//			PumpData_1->creditCard_1,
	//			PumpData_1->creditCard_2,
	//			PumpData_1->creditCard_3,
	//			PumpData_1->creditCard_4,
	//			PumpData_1->fuelType,
	//			PumpData_1->fuelAmount);

	//		CS1->Signal();
	//	}
	//	else if (PS2->Read() > 0) {

	//		PS2->Wait();

	//		printf("Pipeline %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
	//			PumpData_2->pumpID,
	//			MAX_NAME_LENGTH,
	//			PumpData_2->userName,
	//			PumpData_2->creditCard_1,
	//			PumpData_2->creditCard_2,
	//			PumpData_2->creditCard_3,
	//			PumpData_2->creditCard_4,
	//			PumpData_2->fuelType,
	//			PumpData_2->fuelAmount);

	//		CS2->Signal();
	//	}
	//	else if (PS3->Read() > 0) {

	//		PS3->Wait();

	//		printf("Pipeline %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
	//			PumpData_3->pumpID,
	//			MAX_NAME_LENGTH,
	//			PumpData_3->userName,
	//			PumpData_3->creditCard_1,
	//			PumpData_3->creditCard_2,
	//			PumpData_3->creditCard_3,
	//			PumpData_3->creditCard_4,
	//			PumpData_3->fuelType,
	//			PumpData_3->fuelAmount);

	//		CS3->Signal();
	//	}
	//	else if (PS4->Read() > 0) {

	//		PS4->Wait();

	//		printf("Pipeline %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
	//			PumpData_4->pumpID,
	//			MAX_NAME_LENGTH,
	//			PumpData_4->userName,
	//			PumpData_4->creditCard_1,
	//			PumpData_4->creditCard_2,
	//			PumpData_4->creditCard_3,
	//			PumpData_4->creditCard_4,
	//			PumpData_4->fuelType,
	//			PumpData_4->fuelAmount);

	//		CS4->Signal();
	//	}
	//	else {
	//		continue;
	//	}
	//}


	return 0;
}