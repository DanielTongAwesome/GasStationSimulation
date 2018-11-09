#include <stdio.h>
#include <iostream>
#include "..\rt.h"
#include "constants.h"
#include "Customer.h"
#include "..\PumpControl\Pump.h"

int main() {
	
	//string pump_exe_file_path = "C:\\Users\\willi\\OneDrive\\CPEN333_Lab\\GasStationSimulation\\Debug\\";	
	string pump_exe_file_path = "C:\\Users\\13358\\Dropbox\\CPEN333Windows\\GasStationSimulation\\Debug\\";		


	string pump_exec = "PumpControl.exe";
	string GSC_exec = "GasStationComputer.exe";
	// run the process -- Pump
	CProcess Pump_main(pump_exe_file_path + pump_exec,
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window					
		ACTIVE							// process is active immediately
	);
	// run the process -- GSC
	CProcess GSC_main(pump_exe_file_path + GSC_exec,
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window					
		ACTIVE							// process is active immediately
	);

	// initial customer pointer
	Customer *car[NUMBER_OF_CUSTOMER];

	// create each customer
	for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
		SLEEP(100);
		car[i] = new Customer((i%NUMBER_OF_PUMPS + 1));
		//car[i]->Resume();
	}
	
	// resume each customer 
	for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
		car[i]->Resume();
	}


	// wait the pump process to terminate
	Pump_main.WaitForProcess();

	system("PAUSE");
	return 0;
}