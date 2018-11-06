#include "FuelTank.h"

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

FuelTank::~FuelTank()
{
	delete tank;
	delete tankDataPool;
	delete FuelTankMutex;
}

void FuelTank::setPrice(int fueltype, float price)
{
	FuelTankMutex->Wait();
	tank->fuel_price[fueltype - 1] = price;
	FuelTankMutex->Signal();
}

float FuelTank::getPrice(int fueltype)
{
	float currentPrice;
	FuelTankMutex->Wait();
	currentPrice = tank->fuel_price[fueltype - 1];
	FuelTankMutex->Signal();
	return currentPrice;
}
//
//void FuelTank::dispense(int fueltype)
//{
//}

void FuelTank::refilling(int fueltype)
{
	FuelTankMutex->Wait();
	tank->fuel_tank_level[fueltype - 1] = TANK_SIZE;
	FuelTankMutex->Signal();
}

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

bool FuelTank::decrement(int fueltype)
{
	FuelTankMutex->Wait();
	bool status = FALSE;
	tank->fuel_tank_level[fueltype - 1] -= PUMP_RATE;
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
