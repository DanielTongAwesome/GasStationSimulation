#define _CRT_SECURE_NO_WARNINGS
#include "GSC.h"
#include "FuelTank.h"
//#include <time.h>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <list>

using namespace std;

// dispense array
bool dispense[NUMBER_OF_PUMPS];
bool dispenseReject[NUMBER_OF_PUMPS];
bool dispenseStatus[NUMBER_OF_TANKS] = {true, true, true, true};

// initialize global bank 
FuelTank *Fuel_Tank = new FuelTank();

// initialize list transaction histotry
list<struct purchaseInfo> purchaseHistoryList;

// screen control mutex
CMutex *screenControl = new CMutex("GSCScreen");

// constructor
GSC::GSC() {

}

// destuctor
GSC::~GSC() {

}


// clear line function
// clear the line at certain line number
void clearLine(int lineNum) {

	screenControl->Wait();
	MOVE_CURSOR(0, lineNum);
	printf("                                                                                                ");
	MOVE_CURSOR(0, lineNum);
	screenControl->Signal();
}


// start up screen function
void setScreen() {
	CLEAR_SCREEN();
	CURSOR_OFF();
	TEXT_COLOUR(3, 0);
	MOVE_CURSOR(0, 0);
	printf("====================================================================================================\n");
	printf("|--------------------                GAS STATION COMPUTER  V 1.0                -------------------|\n");
	printf("====================================================================================================\n\n\n");
	TEXT_COLOUR(3, 0);
	printf("FUEL TANKS\n");
	printf("  OCT82               OCT87              OCT92                 OCT97\n\n");
	//add a move cursor here to change the fuel amount in the tank
	printf("  %dL                %dL                %dL                 %dL\n", 500, 500, 500, 500);
	printf("  $1.0                $1.1                $1.2                 $1.3\n\n");
	//add if statement, if the fuel amount is lower than 200, blink the LED and suspend the pump
	//Display the message if the total amount of the fuel in the gas station is lower than 500
	TEXT_COLOUR(3, 0);
	//printf("Hello ------ GSC \n");
	printf("PUMPs Status \n\n");
	MOVE_CURSOR(0, 40);
	TEXT_COLOUR(3, 0);
	printf("Please Enter Commands:\n");      //line 41
	//should replace the constant number with pumpID
	printf("D + 1 --- dispense fuel to Pump 1\n");                     // line 42
	printf("R + 1 --- refuse to dispense fuel to Pump 1\n");           // line 43
	printf("F + 1 --- refill the fuel tank 1.\n");   // line 44
	//this function should be modified, like including the fuel price change function
	printf("C + 1 --- change the cost of fuel type 1\n");                // line 45       
	printf("  T   --- display the transactions history\n");              // line 46

	TEXT_COLOUR(12, 0);
	printf("PLEASE ENTER YOUR COMMAND HERE:\n");                         // line 47
	TEXT_COLOUR(3, 0);
}

void ReadKey()
{
	// initialize it with an unrealted 'x'
	char command1 = 'x';
	char command2;
	float command3;
	
	
	// read first input command
	if (TEST_FOR_KEYBOARD() == 1) {
		screenControl->Wait();
		clearLine(47);
		clearLine(48);
		clearLine(49);
		MOVE_CURSOR(0, 47);
		std::cin >> command1;
		screenControl->Signal();

	}

	// Enable dispense
	if (command1 == 'd' || command1 == 'D') 
	{
		screenControl->Wait();
		MOVE_CURSOR(0, 49);
		//printf("Key press D received ... Please enter the Pump No. \n");
		screenControl->Signal();
		
		// read second input command
		std::cin >> command2;
		
		// check the command 2
		switch (command2) 
		{
			// case 1 --> dispense pump 1
			case '1':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 1 been selected\n";
				dispense[0] = true; 
				screenControl->Signal();
				break;
			// case 2 --> dispense pump 2
			case '2':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 2 been selected\n";
				dispense[1] = true;
				screenControl->Signal();
				break;
			// case 3 --> dispense pump 3
			case '3':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 3 been selected\n";
				dispense[2] = true; 
				screenControl->Signal();
				break;
			// case 4 --> dispense pump 4
			case '4':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 4 been selected\n";
				dispense[3] = true;
				screenControl->Signal();
				break;
			// case 5 --> dispense pump 5
			case '5':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 5 been selected\n";
				dispense[4] = true;
				screenControl->Signal();
				break;
			// default case
			default: 
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Please reselect the pump .... \n";
				screenControl->Signal();
				break;
		}
		clearLine(49);
	}//Enable dispense if condition end

	// Rejcect dispense
	else if (command1 == 'r' || command1 == 'R') 
	{
		screenControl->Wait();
		MOVE_CURSOR(0, 49);
		//printf("Key press Q received ... Please enter the Pump No. \n");
		screenControl->Signal();

		// read second input command
		std::cin >> command2;
		switch (command2)
		{
			// case 1 --- reject pump 1 consumer
			case '1':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 1 been rejected\n";
				dispenseReject[0] = true;
				screenControl->Signal();
				break;
			// case 2 --- reject pump 2 consumer
			case '2':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 2 been rejected\n";
				dispenseReject[1] = true;
				screenControl->Signal();
				break;
			// case 3 --- reject pump 3 consumer
			case '3':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 3 been rejected\n";
				dispenseReject[2] = true;
				screenControl->Signal();
				break;
			// case 4 --- reject pump 4 consumer
			case '4':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 4 been rejected\n";
				dispenseReject[3] = true;
				screenControl->Signal();
				break;
			// case 5 --- reject pump 5 consumer
			case '5':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Pump 5 been rejected\n";
				dispenseReject[4] = true;
				screenControl->Signal();
				break;
			// default case
			default:
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Please reselect the pump .... \n";
				screenControl->Signal();
				break;
		}
		clearLine(49);
	} //Rejcect if condition end

	// Note: ( (pumpData->fuelType-1)*20+2, 8 )
	// Refill command
	else if (command1 == 'f' || command1 == 'F') 
	{
		screenControl->Wait();
		MOVE_CURSOR(0, 49);
		//printf("Key press R received ... Please enter the Fuel Tank No. \n");
		screenControl->Signal();

		// read second input command
		std::cin >> command2;
		switch (command2)
		{
			// case 1 ----- Tank 1 refill
			case '1':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Tank 1 been Refilled\n";
				MOVE_CURSOR(2, 8);
				TEXT_COLOUR(10, 0);
				printf("500.0L");
				Fuel_Tank->refilling(FUEL82);
				dispenseStatus[0] = true;
				screenControl->Signal();
				break;
			// case 2 ----- Tank 2 refill
			case '2':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Tank 2 been Refilled\n";
				MOVE_CURSOR(22, 8);
				TEXT_COLOUR(10, 0);
				printf("500.0L");
				Fuel_Tank->refilling(FUEL87);
				dispenseStatus[1] = true;
				screenControl->Signal();
				break;
			// case 3 ----- Tank 3 refill
			case '3':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Tank 3 been Refilled\n";
				MOVE_CURSOR(42, 8);
				TEXT_COLOUR(10, 0);
				printf("500.0L");
				Fuel_Tank->refilling(FUEL92);
				dispenseStatus[2] = true;
				screenControl->Signal();
				break;
			// case 4 ----- Tank 4 refill
			case '4':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Tank 4 been Refilled\n";
				MOVE_CURSOR(62, 8);
				TEXT_COLOUR(10, 0);
				printf("500.0L");
				Fuel_Tank->refilling(FUEL97);
				dispenseStatus[3] = true;
				screenControl->Signal();
				break;
			// default case
			default:
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Please reselect the Fuel Tank .... \n";
				screenControl->Signal();
				break;
		}
		clearLine(49);
	}// Refill if condition end


	// Change cost command 
	else if (command1 == 'c' || command1 == 'C') 
	{
		screenControl->Wait();
		MOVE_CURSOR(0, 49);
		//printf("Key press C received ... Please select the Fuel Type you want to change the price \n");
		screenControl->Signal();

		// read second input command
		std::cin >> command2;
		switch (command2)
		{
			// case 1 ---- change the price of fueltype 1 
			case '1':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
			
				while (TEST_FOR_KEYBOARD() == 0) {
					continue;
				}
				clearLine(49);
				screenControl->Wait();
				MOVE_CURSOR(0, 47);
				std::cin >> command3;
				screenControl->Signal();
				//
				Fuel_Tank->setPrice(FUEL82, command3);
				std::cout << "Price of Fuel Type 82 changed to "  << command3 << "\n";
				MOVE_CURSOR(2, 9);
				printf("$%.2f", command3);
				screenControl->Signal();
				break;
			// case 2 ---- change the price of fueltype 2 
			case '2':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				//std::cout << "Please enter the price of Fuel Type 87 \n";
				//
				while (TEST_FOR_KEYBOARD() == 0) {
					continue;
				}
				clearLine(49);
				screenControl->Wait();
				MOVE_CURSOR(0, 47);
				std::cin >> command3;
				screenControl->Signal();
				//
				Fuel_Tank->setPrice(FUEL87, command3);
				std::cout << "Price of Fuel Type 87 changed to "  << command3 << "\n";
				MOVE_CURSOR(22, 9);
				printf("$%.2f", command3);
				screenControl->Signal();
				break;
			// case 3 ---- change the price of fueltype 3 
			case '3':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				//std::cout << "Please enter the price of Fuel Type 92 \n";
				//
				while (TEST_FOR_KEYBOARD() == 0) {
					continue;
				}
				clearLine(49);
				screenControl->Wait();
				MOVE_CURSOR(0, 47);
				std::cin >> command3;
				screenControl->Signal();
				//
				Fuel_Tank->setPrice(FUEL92, command3);
				std::cout << "Price of Fuel Type 92 changed to "  << command3 << "\n";
				MOVE_CURSOR(42, 9);
				printf("$%.2f", command3);
				screenControl->Signal();
				break;
			// case 4 ---- change the price of fueltype 4 
			case '4':
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				//std::cout << "Please enter the price of Fuel Type 97 \n";
				//
				while (TEST_FOR_KEYBOARD() == 0) {
					continue;
				}
				clearLine(49);
				screenControl->Wait();
				MOVE_CURSOR(0, 47);
				std::cin >> command3;
				screenControl->Signal();
				//
				Fuel_Tank->setPrice(FUEL97, command3);
				std::cout << "Price of Fuel Type 97 changed to "  << command3 << "\n";
				MOVE_CURSOR(63, 9);
				printf("$%.2f", command3);
				screenControl->Signal();
				break;
			// default case
			default:
				screenControl->Wait();
				MOVE_CURSOR(0, 49);
				clearLine(49);
				std::cout << "Please reselect the Fuel Type from 1 to 4 .... \n";
				screenControl->Signal();
				break;
		}
		clearLine(49);
	} // Change cost if condition end 

	// show transaction history
	else if (command1 == 't' || command1 == 'T')
	{
		screenControl->Wait();
		MOVE_CURSOR(0, 51);
		TEXT_COLOUR(3, 0);
		clearLine(51);
		cout << "==================================================" << endl;
		cout << "               Transaction History                " << endl;
		cout << "==================================================" << endl;

		// create iterator
		list<struct purchaseInfo>::iterator i;
		// initlaize customer No
		int customer_number = 0;

		// check if the list is empty or not
		if (purchaseHistoryList.begin() == purchaseHistoryList.end() ) {
			cout << "Empty --- There is no transaction history !" << endl;
		}

		// if not empty then print each one
		for (i = purchaseHistoryList.begin(); i != purchaseHistoryList.end(); ++i)
		{
			customer_number++;
			cout << "Cutomer No."			<< customer_number << ":" << endl;
			cout << "Name: "				<< i->userName << endl;			
			cout << "Credit card number: "  << i->creditCard_1 << " " << i->creditCard_2 << " " << i->creditCard_3 << " " << i->creditCard_4 << endl;
			cout << "FuelType: "			<< i->fuelType << endl;
			cout << "Dispense Fuel: "		<< i->dispensedFuel << endl;
			cout << "Selected Fuel Price: "	<< i->SelectedFuelPrice << endl;
			cout << "Purchased Time: " << asctime(localtime(&i->purchaseTime)) << endl;
			cout << endl;
		}
		screenControl->Signal();

	} // show transaction history if condition end
} //void ReadKey() end 

// this thread update the pump user status
UINT __stdcall pump_user_status_thread(void *args) {

	int Thread_Number = *(int *)(args);
	int colorIndex = 0;
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

		screenControl->Wait();
		MOVE_CURSOR(0, 10 + (4*pumpData->pumpID));
		TEXT_COLOUR(3, 0);
		// retrieve the customer infomation
		printf("[- Pump %d -] Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
			pumpData->pumpID,
			MAX_NAME_LENGTH,
			pumpData->userName,
			pumpData->creditCard_1,
			pumpData->creditCard_2,
			pumpData->creditCard_3,
			pumpData->creditCard_4,
			pumpData->fuelType,
			pumpData->fuelAmount);

		screenControl->Signal();

		// forever loop at here when GSC haven't authorise or reject it
		while ((dispense[Thread_Number - 1] == false) && (dispenseReject[Thread_Number - 1] == false)) 
		{
			continue;
		} 


		// forever loop at here when fuel tank is below MIN_LEVEL or havenot reject it
		while (dispenseStatus[pumpData->fuelType - 1] == false && dispenseReject[Thread_Number - 1] == false)
		{
			screenControl->Wait();
			TEXT_COLOUR(10, 0);
			MOVE_CURSOR(0, 15 + 4 * (pumpData->pumpID - 1));
			clearLine(15 + 4 * (pumpData->pumpID - 1));
			TEXT_COLOUR(12, 0);
			// print this msg to let the user know the fuel tank level is insuffcient
			printf("Fuel Tank Fuel Type %d Below 200 L, Please Refill .... \n", pumpData->fuelType);
			clearLine(16 + 4 * (pumpData->pumpID - 1));
			fflush(stdout);
			screenControl->Signal();
			// if the fuel tank is now higher than the MIN_LEVEL clear alert
			if (Fuel_Tank->readFuelLevel(pumpData->fuelType) > MIN_LEVEL) {
				clearLine(15 + 4 * (pumpData->pumpID - 1));
				break;
			}
			
			// refill
			SLEEP(1000);
		}

		// printf("While Loop break\n");
		// when gas station attendant gives a command to pump
		if (dispense[Thread_Number - 1] == true)
		{
			pumpData->SelectedFuelPrice = Fuel_Tank->getPrice(pumpData->fuelType);
			//printf("the price is : %f \n", pumpData->SelectedFuelPrice);

			pumpData->dispense_enable = 1;
			//printf("pump%d is dispending fuel \n", Thread_Number);
			dispense[Thread_Number - 1] = false;
		}

		// if command reject been detected
		else if (dispenseReject[Thread_Number - 1] == true)
		{
			pumpData->reject_enable = 1;
		}

		
		CS->Signal();



		// Updated the Gas Computer how much oil has been dispensed and the cost
		while (GSCPumpCost->Read() == 0 && dispenseReject[Thread_Number - 1] == false) // if the pump havn't finished dispending
		{
			PS->Wait();

			screenControl->Wait();
			TEXT_COLOUR(6, 0);
			MOVE_CURSOR(0, 15 + 4*(pumpData->pumpID-1));
			clearLine(15 + 4 * (pumpData->pumpID - 1));              // clear line after [pump No]
			printf("dispensed Fuel is %.1f, and cost is $%.2f  \n", pumpData->dispensedFuel, pumpData->cost);
			clearLine(15 + 4 * (pumpData->pumpID - 1) + 1);			 // clear the gap between two [pump]
			dispenseStatus[pumpData->fuelType - 1] = Fuel_Tank->decrement(pumpData->fuelType);
			//printf("Fuel Tank ---- Type %d  Remains %f\n", pumpData->fuelType, Fuel_Tank->readFuelLevel(pumpData->fuelType));
			MOVE_CURSOR((pumpData->fuelType-1)*20+2, 8);
			//printf("%.1fL", Fuel_Tank->readFuelLevel(pumpData->fuelType));
			if (Fuel_Tank->readFuelLevel(pumpData->fuelType) < MIN_LEVEL) {
				TEXT_COLOUR(12, 0);
			}
			else if (Fuel_Tank->readFuelLevel(pumpData->fuelType) >= MIN_LEVEL) {
				TEXT_COLOUR(10, 0);
			}
			printf("%.1fL", Fuel_Tank->readFuelLevel(pumpData->fuelType));
			fflush(stdout);
			screenControl->Signal();
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
				// transaction history list temp
				struct purchaseInfo purchase_temp;
				strcpy_s(purchase_temp.userName, pumpData->userName);
				purchase_temp.creditCard_1 = pumpData->creditCard_1;
				purchase_temp.creditCard_2 = pumpData->creditCard_2;
				purchase_temp.creditCard_3 = pumpData->creditCard_3;
				purchase_temp.creditCard_4 = pumpData->creditCard_4;
				purchase_temp.fuelType     = pumpData->fuelType;
				purchase_temp.dispensedFuel = pumpData->dispensedFuel;
				purchase_temp.SelectedFuelPrice = pumpData->SelectedFuelPrice;
				purchase_temp.purchaseTime = pumpData->purchaseTime;
				screenControl->Wait();
				TEXT_COLOUR(10, 0);
				MOVE_CURSOR(0, 15 + 4 * (pumpData->pumpID - 1));
				clearLine(15 + 4 * (pumpData->pumpID - 1));
				printf("Purchase has been recorded ... \n");
				fflush(stdout);
				screenControl->Signal();
				// read the purchase history and store it into the temp purchase_temp
				purchaseHistoryList.push_back(purchase_temp);
				GSCPumpCost->Wait();
				break;
			}
		}
		

		// process the GSC reject
		if (dispenseReject[Thread_Number - 1] == true)
		{
			PS->Wait();

			screenControl->Wait();
			TEXT_COLOUR(10, 0);
			MOVE_CURSOR(0, 15 + 4 * (pumpData->pumpID - 1));
			clearLine(15 + 4 * (pumpData->pumpID - 1));
			printf("pump%d rejects customer to fuel \n", Thread_Number);
			fflush(stdout);
			screenControl->Signal();

			CS->Signal();
			dispenseReject[Thread_Number - 1] = false;
		}
		

		screenControl->Wait();
		TEXT_COLOUR(10, 0);
		MOVE_CURSOR(0, 16 + 4 * (pumpData->pumpID - 1));
		clearLine(16 + 4 * (pumpData->pumpID - 1));
		printf("Customer %-*s is leaving the pump \n", MAX_NAME_LENGTH, pumpData->userName);
		fflush(stdout);
		screenControl->Signal();
		
	}

	//return 0;
}


// active class main thread
int GSC::main(void) {

	// start up call
	setScreen();

	// create thread number
	int ThreadNum[NUMBER_OF_PUMPS] = {1,2,3,4,5};
	CThread *Datapool_Thread[NUMBER_OF_PUMPS]; 

	// run each thread
	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		Datapool_Thread[i] = new CThread(pump_user_status_thread, SUSPENDED, &ThreadNum[i]);
		Datapool_Thread[i]->Resume();
	}

	
	// read key 
	while (1) {
		ReadKey();
	}

	// you can ignore the below lines
	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		Datapool_Thread[i]->WaitForThread();
	}
	
	return 0;
}