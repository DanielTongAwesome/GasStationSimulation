#include "Customer.h"
#include <ctime>
#include <string>
#include <sstream>
#include <stdio.h>

const char* names_array[] = { "Daniel", "Nancy", "CoolGuy", "Ben", "charlie", "Megan",
"Spidermen", "Batmen", "Funnykids",  "Bugs", "Fatima", "Steven", "Luis","Dylan", "James",
"Ramon", "Noah", "Nathan", "Geek", "Diego", "Carlos", "William", "Logan", "Robert", "Gilbert",
"Hilbert", "Juan", "Piggy", "Lovelykids", "Funnyboys", "Freemenson", "BadBoy","Street", "Agustin",
"Olivia", "Oliver", "Liu", "Tong", "Tiger", "Sophia", "Random", "Frank", "Underwood", "MK",
"Sofia", "Lura", "Muhamad", "UnkownBoy", "Jimmy", "David" };

using namespace std;

Customer::Customer(int go_to_pump)
{
	// assign each car with a pump number
	destination_pump_number = go_to_pump;

	// credit card info 
	//srand(time(NULL));
	customerData.creditCard = rand() % 10000000000000000;

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
	printf("Creating customer %s and queing at Pump %d...\n", customerData.name, destination_pump_number);
	// set up destination pump
	std::ostringstream oss;
	string pump = "Pump";
	oss << destination_pump_number;
	pump += oss.str();

	// set up pipline to talk with pump and mutex
	pipeline = new CTypedPipe<struct customerInfo>(pump, 1);
	/*pipelineMutex = new CMutex(pump);
	pump = "Pump2";
	pump += oss.str();
	pipelineMutex2 = new CMutex(pump);*/
	EntryGate = new CSemaphore(pump + "EntryGate", 0, 1);
	ExitGate = new CSemaphore(pump + "ExitGate", 0, 1);
	Full = new CSemaphore(pump + "Full", 0, 1);
	Empty = new CSemaphore(pump + "Empty", 0, 1);


	// set up semaphore for entering the pump
	// create semaphore for each car waitting on the line
	/*
	string EnteryGateAddress = "EnteryGate";
	string ExistGateAddress = "ExistGate";
	string FullAddress = "Full";
	string EmptyAddress = "Empty";
	EnteryGateAddress += oss.str();
	ExistGateAddress += oss.str();
	FullAddress += oss.str();
	EmptyAddress += oss.str();
	
	EntryGate = new CSemaphore(EnteryGateAddress, 0, 1);
	ExitGate = new CSemaphore(ExistGateAddress, 0, 1);
	Empty = new CSemaphore(EmptyAddress, 0, 1);
	Full = new CSemaphore(FullAddress, 0, 1);
	*/
}



int Customer::main(void)
{
	// waitting to use the pump
	//EntryGate->Wait();
	//Full->Signal();

	while (1) {
		
		// traffic logic: entering the pump
		EntryGate->Wait();
		Full->Signal();

		printf("I am customer %s, want to use the pump %d \n", customerData.name, destination_pump_number);

		pipeline->Write(&customerData);
		SLEEP(5000);

		printf("Customer %s has swiped his card at Pump %d ... \n", customerData.name, destination_pump_number);
		Sleep(1000);
		if (customerData.fuelType == FUEL82)
			printf("Customer %s has selected fuel grade OCT82... \n", customerData.name);
		else if (customerData.fuelType == FUEL87)
			printf("Customer %s has selected fuel grade OCT87... \n", customerData.name);
		else if (customerData.fuelType == FUEL92)
			printf("Customer %s has selected fuel grade OCT92... \n", customerData.name);
		else if (customerData.fuelType == FUEL92)
			printf("Customer %s has selected fuel grade OCT97... \n", customerData.name);
		Sleep(1000);
		
		// traffic logic: exiting the pump
		ExitGate->Wait();
		Empty->Signal();

		// printing indicate customer is leaving 
		printf("Customer %s is leaving the pump %d \n", customerData.name, destination_pump_number);
	}



	return 0;
}


Customer::~Customer(void)
{
	/*delete pipelineMutex;
	delete pipelineMutex2;*/
	delete EntryGate;
	delete ExitGate;
	delete Full;
	delete Empty;
}
