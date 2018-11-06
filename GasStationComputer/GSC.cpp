#include "GSC.h"
#include "FuelTank.h"
#include <ctime>
#include <string>
#include <sstream>
#include <stdio.h>

// dispense array
bool dispense[NUMBER_OF_PUMPS];
bool dispenseReject[NUMBER_OF_PUMPS];

// initialize global bank 
FuelTank *Fuel_Tank = new FuelTank();

GSC::GSC() {

}

GSC::~GSC() {

}




void ReadKey()
{
	char command1;
	char command2;
	float command3;
	
	// read first input command
	cin >> command1;

	// Enable dispense
	if (command1 == 'd' || command1 == 'D') 
	{
		printf("Key press D received ... Please enter the Pump No. \n");
		
		// read second input command
		cin >> command2;
		switch (command2) {
		case '1':	
			std::cout << "Pump 1 been selected\n";
			dispense[0] = true; 
			break;
		case '2': 	
			std::cout << "Pump 2 been selected\n";
			dispense[1] = true; 
			break;
		case '3': 	
			std::cout << "Pump 3 been selected\n";
			dispense[2] = true; 
			break;
		case '4': 	
			std::cout << "Pump 4 been selected\n";
			dispense[3] = true; 
			break;
		default: 
			std::cout << "Please reselect the pump .... \n";
			break;
		}		
	}//Enable dispense if condition end

	// Rejcect dispense
	else if (command1 == 'q' || command1 == 'Q') 
	{
		printf("Key press Q received ... Please enter the Pump No. \n");

		// read second input command
		cin >> command2;
		switch (command2)
		{
		case '1':
			std::cout << "Pump 1 been rejected\n";
			dispenseReject[0] = true;
			break;
		case '2':
			std::cout << "Pump 2 been rejected\n";
			dispenseReject[1] = true;
			break;
		case '3':
			std::cout << "Pump 3 been rejected\n";
			dispenseReject[2] = true;
			break;
		case '4':
			std::cout << "Pump 4 been rejected\n";
			dispenseReject[3] = true;
			break;
		default:
			std::cout << "Please reselect the pump .... \n";
			break;
		}
	} //Rejcect if condition end


	// Refill command
	else if (command1 == 'r' || command1 == 'R') 
	{
			printf("Key press R received ... Please enter the Fuel Tank No. \n");

			// read second input command
			cin >> command2;
			switch (command2)
			{
			case '1':
				std::cout << "Tank 1 been Refilled\n";
				Fuel_Tank->refilling(FUEL82);
				break;
			case '2':
				std::cout << "Tank 2 been Refilled\n";
				Fuel_Tank->refilling(FUEL87);
				break;
			case '3':
				std::cout << "Tank 3 been Refilled\n";
				Fuel_Tank->refilling(FUEL92);
				break;
			case '4':
				std::cout << "Tank 4 been Refilled\n";
				Fuel_Tank->refilling(FUEL97);
				break;
			default:
				std::cout << "Please reselect the Fuel Tank .... \n";
				break;
			}
	}// Refill if condition end

	// Change cost command 
	else if (command1 == 'c' || command1 == 'C') 
	{
			printf("Key press C received ... Please select the Fuel Type you want to change the price \n");

			// read second input command
			cin >> command2;
			switch (command2)
			{
			case '1':
				std::cout << "Please enter the price of Fuel Type 82 \n";
				cin >> command3;
				Fuel_Tank->setPrice(FUEL82, command3);
				std::cout << "Price of Fuel Type 82 changed to "  << command3 << "\n";
				std::cout << Fuel_Tank->getPrice(1) << "\n";
				break;
			case '2':
				std::cout << "Please enter the price of Fuel Type 87 \n";
				cin >> command3;
				Fuel_Tank->setPrice(FUEL87, command3);
				std::cout << "Price of Fuel Type 87 changed to "  << command3 << "\n";
				break;
			case '3':
				std::cout << "Please enter the price of Fuel Type 92 \n";
				cin >> command3;
				Fuel_Tank->setPrice(FUEL92, command3);
				std::cout << "Price of Fuel Type 92 changed to "  << command3 << "\n";
				break;
			case '4':
				std::cout << "Please enter the price of Fuel Type 97 \n";
				cin >> command3;
				Fuel_Tank->setPrice(FUEL97, command3);
				std::cout << "Price of Fuel Type 97 changed to "  << command3 << "\n";
				break;
			default:
				std::cout << "Please reselect the Fuel Type from 1 to 4 .... \n";
				break;
			}
	} // Change cost if condition end 

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

		// retrieve the customer infomation
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
		while ((dispense[Thread_Number - 1] == false) && (dispenseReject[Thread_Number - 1] == false)) 
		{
			continue;
		} 
		// printf("While Loop break\n");
		// when gas station attendant gives a command to pump
		if (dispense[Thread_Number - 1] == true)
		{
			pumpData->SelectedFuelPrice = Fuel_Tank->getPrice(pumpData->fuelType);
			printf("the price is : %f \n", pumpData->SelectedFuelPrice);

			pumpData->dispense_enable = 1;
			printf("pump%d is dispending fuel \n", Thread_Number);
			dispense[Thread_Number - 1] = false;
		}

		else if (dispenseReject[Thread_Number - 1] == true)
		{
			pumpData->reject_enable = 1;
			printf("pump%d rejects customer to fuel \n", Thread_Number);
		}

		
		CS->Signal();



		// Updated the Gas Computer how much oil has been dispensed and the cost
		while (GSCPumpCost->Read() == 0 && dispenseReject[Thread_Number - 1] == false) // if the pump havn't finished dispending
		{
			PS->Wait();
			printf("dispensed Fuel is %.1f, and cost is %.1f  \n", pumpData->dispensedFuel, pumpData->cost);
			// debug purpose
			//printf("GSCPumpCost->Read() is:  %d \n", GSCPumpCost->Read());
			//printf("Customer %-*s  \n", MAX_NAME_LENGTH, pumpData->userName);
			CS->Signal();
			// Note: Here is an important break statement
			// Each time after cs->signal, we should check whether the fuelamount and dispensedfuel are equal or not
			// If they are equal, then we finished the fill up process, then we should break the while loop
			// other wise program will go back to ps->wait and give error output 
			if (pumpData->dispensedFuel == pumpData->fuelAmount)
			{
				GSCPumpCost->Wait();
				break;
			}
		}
		

		// process the GSC reject
		if (dispenseReject[Thread_Number - 1] == true)
		{
			PS->Wait();
			printf("pump%d rejects customer to fuel \n", Thread_Number);
			CS->Signal();
			dispenseReject[Thread_Number - 1] = false;
		}
		

		

		printf("Customer %-*s is leaving the pump \n", MAX_NAME_LENGTH, pumpData->userName);
		
		
	}

	//return 0;
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