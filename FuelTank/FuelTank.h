#ifndef __FuelTank__
#define __FuelTank__

#include <stdio.h>
#include "..\rt.h"
#include "..\GasStationSimulation\constants.h"

// class for FuelTank --- ActiveClass
class FuelTank : public ActiveClass {

private:
	int fuel_tank_level = MAX_TANK_LITRES; // TODO - should actually put this in a datapool
	CMutex *fuel_tank_mutex;
	CDataPool *fuel_tank_datapool;
	

public:
	void refill_tank(/*TODO - make multiple types*/);
	void pump_decrement();
	int read_tank_value();

};

#endif
