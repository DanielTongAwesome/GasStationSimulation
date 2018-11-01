#include "Customer.h"
#include <ctime>
#include <string>
#include <sstream>
#include <stdio.h>



// name array: select names from the following array
const char* names_array[] = { "Daniel", "Nancy", "CoolGuy", "Ben", "charlie", "Megan",
"Spidermen", "Batmen", "Funnykids",  "Bugs", "Fatima", "Steven", "Luis","Dylan", "James",
"Ramon", "Noah", "Nathan", "Geek", "Diego", "Carlos", "William", "Logan", "Robert", "Gilbert",
"Hilbert", "Juan", "Piggy", "Lovelykids", "Funnyboys", "Freemenson", "BadBoy","Street", "Agustin",
"Olivia", "Oliver", "Liu", "Tong", "Tiger", "Sophia", "Random", "Frank", "Underwood", "MK",
"Sofia", "Lura", "Muhamad", "UnkownBoy", "Jimmy", "David" };

using namespace std;


// customer constructor
Customer::Customer(int go_to_pump)
{
	// assign each car with a pump number
	destination_pump_number = go_to_pump;

	// credit card info 
	//srand(time(NULL));
	customerData.creditCard_1 = rand() % 9000 + 1000;
	customerData.creditCard_2 = rand() % 9000 + 1000;
	customerData.creditCard_3 = rand() % 9000 + 1000;
	customerData.creditCard_4 = rand() % 9000 + 1000;


	// assign names to each car user
	//srand(time(NULL));
	int choosen_pos = rand() % 40;
	strcpy_s(customerData.name, names_array[choosen_pos]);

	// fuel and fuel amount
	//srand(time(NULL));
	int temp = rand() % 4;
	if (temp == 0) {
		customerData.fuelType = FUEL82;
	}
	else if (temp == 1) {
		customerData.fuelType = FUEL87;
	}
	else if (temp == 2) {
		customerData.fuelType = FUEL92;
	}
	else if (temp == 3) {
		customerData.fuelType = FUEL97;
	}

	customerData.fuelAmount = rand() % MAX_AMOUNT;

	// initial print
	printf("Creating customer %-*s and queing at Pump %d...\n", MAX_NAME_LENGTH, customerData.name, destination_pump_number);
	
	// set up destination pump
	std::ostringstream oss;
	string pump = "Pump";
	oss << destination_pump_number;
	pump += oss.str();

	// set up pipline to talk with pump and mutex
	pipeline = new CTypedPipe<struct customerInfo>(pump, 1);
	

	// create semaphores
	EntryGate = new CSemaphore(pump + "EntryGate", 0, 1);
	ExitGate = new CSemaphore(pump + "ExitGate", 0, 1);
	Full = new CSemaphore(pump + "Full", 0, 1);
	Empty = new CSemaphore(pump + "Empty", 0, 1);

	// create GSC Semaphore
	GSCCommand = new CSemaphore(pump+ "GSCCommand", 0, 1);

}



// customer main
int Customer::main(void)
{
	// customer running logic
	while (1) {
		
		// traffic logic: entering the pump
		EntryGate->Wait();
		Full->Signal();


		printf("I am customer %-*s want to use the pump %d \n", MAX_NAME_LENGTH, customerData.name, destination_pump_number);

		// write customer data into pipeline
		pipeline->Write(&customerData);
		SLEEP(2000);

		// customer swip credit card 
		printf("Customer %-*s has swiped his card at Pump %d ... \n", MAX_NAME_LENGTH,customerData.name, destination_pump_number);
		customerData.purchaseTime = std::time(0);
	
		Sleep(2000);
		
		

		// customer select pump
		if (customerData.fuelType == FUEL82)
			printf("Customer %-*s has selected fuel grade OCT82... \n", MAX_NAME_LENGTH, customerData.name);
		else if (customerData.fuelType == FUEL87)
			printf("Customer %-*s has selected fuel grade OCT87... \n", MAX_NAME_LENGTH, customerData.name);
		else if (customerData.fuelType == FUEL92)
			printf("Customer %-*s has selected fuel grade OCT92... \n", MAX_NAME_LENGTH, customerData.name);
		else if (customerData.fuelType == FUEL97)
			printf("Customer %-*s has selected fuel grade OCT97... \n", MAX_NAME_LENGTH, customerData.name);
		Sleep(2000);
		
		// wait the GSC Command
		// printing indicate customer is leaving 
		GSCCommand->Wait();
		printf("Customer %-*s is leaving the pump %d \n", MAX_NAME_LENGTH, customerData.name, destination_pump_number);
	

		// traffic logic: exiting the pump
		ExitGate->Wait();
		Empty->Signal();

	
	}



	return 0;
}


// customer destructor
Customer::~Customer(void)
{
	// delete the created pointer to release memeory
	delete EntryGate;
	delete ExitGate;
	delete Full;
	delete Empty;
}
