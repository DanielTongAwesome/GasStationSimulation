#include <stdio.h>
#include <iostream>
#include "..\rt.h"
#include "constants.h"
#include "Customer.h"
#include "..\PumpControl\Pump.h"

int main() {
	
	
	/* test code for customer pipeline */
	/*Customer *car[1];
	car[1] = new Customer(1);
	car[1]->Resume();*/

	/* final code for customer generation and line up*/

	string pump_exe_file_path = "C:\\Users\\13358\\Dropbox\\CPEN333Windows\\GasStationSimulation\\Debug\\";
	string pump_exec = "PumpControl.exe";
	CProcess Pump_main(pump_exe_file_path + pump_exec,
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window					
		ACTIVE							// process is active immediately
	);

	Customer *car[NUMBER_OF_CUSTOMER];

	for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
		SLEEP(100);
		car[i] = new Customer((i%NUMBER_OF_PUMPS + 1));
		//car[i]->Resume();
	}
	
	for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
		car[i]->Resume();
	}


	Pump_main.WaitForProcess();
	system("PAUSE");
	return 0;
}