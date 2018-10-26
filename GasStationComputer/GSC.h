#ifndef __GSC__
#define __GSC__

#include "..\rt.h"
#include <stdio.h>
#include "..\GasStationSimulation\\constants.h"

class GSC : public ActiveClass {

private:
	// some varaiable to define the pump address
	string pumpName_1 = "Pump1";
	string pumpName_2 = "Pump2";
	string pumpName_3 = "Pump3";
	string pumpName_4 = "Pump4";
	string pumpPS_1 = "PS1";
	string pumpCS_1 = "CS1";
	string pumpPS_2 = "PS2";
	string pumpCS_2 = "CS2";
	string pumpPS_3 = "PS3";
	string pumpCS_3 = "CS3";
	string pumpPS_4 = "PS4";
	string pumpCS_4 = "CS4";
	

	// initial data struct for pump
	struct pumpInfo *PumpData_1;
	struct pumpInfo *PumpData_2;
	struct pumpInfo *PumpData_3;
	struct pumpInfo *PumpData_4;

	// 4 datapools for each pump
	CDataPool *datapool1;
	CDataPool *datapool2;
	CDataPool *datapool3;
	CDataPool *datapool4;

	// semaphores for 4 datapools
	CSemaphore *PS1;
	CSemaphore *PS2;
	CSemaphore *PS3;
	CSemaphore *PS4;
	CSemaphore *CS1;
	CSemaphore *CS2;
	CSemaphore *CS3;
	CSemaphore *CS4;

public:
	GSC();
	~GSC();


private:
	int main(void);
};

#endif