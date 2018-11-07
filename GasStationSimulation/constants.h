#ifndef __Constans__
#define __Constans__

#define NUMBER_OF_CUSTOMER 20
#define NUMBER_OF_PUMPS 5
#define NUMBER_OF_TANKS 4
#define TANK_SIZE 500
#define MIN_LEVEL 200
#define PUMP_RATE 0.5f

#define FUEL82 1
#define FUEL82_PRICE 1.459f

#define FUEL87 2
#define FUEL87_PRICE 1.629f

#define FUEL92 3
#define FUEL92_PRICE 1.709f

#define FUEL97 4
#define FUEL97_PRICE 1.881f

#define MAX_AMOUNT 50
#define MAX_NAME_LENGTH 13
#define MAX_TANK_LITRES 2000


// customer information data struct
struct customerInfo {
	char name[MAX_NAME_LENGTH];
	int creditCard_1;
	int creditCard_2;
	int creditCard_3;
	int creditCard_4;
	int fuelType;
	int fuelAmount;
	float SelectedFuelPrice;
	time_t purchaseTime;
};

// pump information data struct
struct pumpInfo {
	int pumpID;
	char userName[MAX_NAME_LENGTH];
	int creditCard_1;
	int creditCard_2;
	int creditCard_3;
	int creditCard_4;
	int fuelType;
	int fuelAmount;
	float SelectedFuelPrice;
	int dispense_enable;
	int reject_enable;
	float dispensedFuel;
	float cost;
	bool dispenseStatus;
	time_t purchaseTime;
};

// fuel tank data struct
struct fuelTankInfo {
	float fuel_tank_level[NUMBER_OF_TANKS];
	float fuel_price[NUMBER_OF_TANKS];
};


// transaction history
struct purchaseInfo {
	char userName[MAX_NAME_LENGTH];
	int creditCard_1;
	int creditCard_2;
	int creditCard_3;
	int creditCard_4;
	int fuelType;
	float dispensedFuel;
	float SelectedFuelPrice;
	time_t purchaseTime;
};

#endif