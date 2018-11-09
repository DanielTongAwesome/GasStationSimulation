#include "FuelTank.h"

// constructor
FuelTank::FuelTank()
{
	FuelTankMutex = new CMutex("FuelTankMutex");
	tankDataPool = new CDataPool("FuelTankDataPool", sizeof(struct fuelTankInfo));
	tank = (struct fuelTankInfo *)(tankDataPool->LinkDataPool());

	// initialize 4 fuel tank and each of them contains 500 L fuel

	for (int i = 0; i < NUMBER_OF_TANKS; i++) {
		tank->fuel_tank_level[i] = TANK_SIZE;
		tank->fuel_price[i] = 1;
	}
}

// destructor
FuelTank::~FuelTank()
{
	delete tank;
	delete tankDataPool;
	delete FuelTankMutex;
}

// set the price of each fueltype
void FuelTank::setPrice(int fueltype, float price)
{
	FuelTankMutex->Wait();
	tank->fuel_price[fueltype - 1] = price;
	FuelTankMutex->Signal();
}

// get the price of each fueltype
float FuelTank::getPrice(int fueltype)
{
	float currentPrice;
	FuelTankMutex->Wait();
	currentPrice = tank->fuel_price[fueltype - 1];
	FuelTankMutex->Signal();
	return currentPrice;
}

// refilling the fill tank
void FuelTank::refilling(int fueltype)
{
	FuelTankMutex->Wait();
	tank->fuel_tank_level[fueltype - 1] = TANK_SIZE;
	FuelTankMutex->Signal();
}

// increase the fuel tank fuel level
bool FuelTank::increment(int fueltype)
{
	FuelTankMutex->Wait();
	bool status = FALSE;
	if (tank->fuel_tank_level[fueltype - 1] < TANK_SIZE) {
		status = TRUE;
		tank->fuel_tank_level[fueltype - 1] += PUMP_RATE;
	}
	FuelTankMutex->Signal();
	return status;
}

// decrease the fuel tank fuel level: used for dispense
bool FuelTank::decrement(int fueltype)
{
	FuelTankMutex->Wait();
	// set status to false
	bool status = FALSE;
	// decrease the fuel tank level first
	tank->fuel_tank_level[fueltype - 1] -= PUMP_RATE;

	// check whether the fuel tank fuel level is lower than MIN_LEVEL or not
	// if yes ----> set it to true
	// if no -----> remain unchange
	if (tank->fuel_tank_level[fueltype - 1] > MIN_LEVEL) {
		status = TRUE;
	}
	FuelTankMutex->Signal();
	return status;
}


// return the current fuel level
float FuelTank::readFuelLevel(int fueltype)
{
	float fuelAmount;
	FuelTankMutex->Wait();
	fuelAmount = tank->fuel_tank_level[fueltype - 1];
	FuelTankMutex->Signal();
	return fuelAmount;
}
