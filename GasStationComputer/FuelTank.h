#include <stdio.h>
#include <iostream>
#include "..\GasStationSimulation\constants.h"
#include "..\rt.h"

#ifndef __FuelTank__
#define __FuelTank__


class FuelTank {
private:
	// create tank data struct
	struct fuelTankInfo *tank; 

	// create tank data pool
	CDataPool *tankDataPool;

	// create fuel tank mutex
	CMutex *FuelTankMutex;
	

public:
	// constructor and destructor
	FuelTank();
	~FuelTank();
	
	void setPrice(int fueltype, float price);  // set price
	float getPrice(int fueltype);			   // get current price
	float readFuelLevel(int fueltype);		   // read current fuel level
	//bool dispense(int fueltype);			   // dispense fuel on the corresponding tank
	void refilling(int fueltype);			   // refill fuel tank of the corresponding tank
	bool increment(int fueltype);			   // increment the fuel
	bool decrement(int fueltype);			   // decrement the fuel
	
};

#endif
