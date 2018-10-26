#include "FuelTank.h"
#include <ctime>
#include <string>
#include <sstream>
#include <stdio.h>

/*
* Since the fuel tanks are based off of monitor classes, each
* fuel tank method must first wait for the mutex before writing
* to the data pool and signal afterwards to unlock the datapool
*/

// Fuel Tank Constructor
FuelTank::FuelTank() {}

// Fuel Tank Destructor
FuelTank::~FuelTank()
{
	// delete the created pointer to release memeory
	//delete EntryGate;
	//delete ExitGate;
	//delete Full;
	//delete Empty;

}

void FuelTank::refill_tank() {
	fuel_tank_mutex->Wait();
	fuel_tank_level = MAX_TANK_LITRES;
	fuel_tank_mutex->Signal();
}

void FuelTank::pump_decrement() {
	fuel_tank_mutex->Wait();
	fuel_tank_level--;
	fuel_tank_mutex->Signal();
}

int FuelTank::read_tank_value() {
	return fuel_tank_level;
}