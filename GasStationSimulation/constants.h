#ifndef __Constans__
#define __Constans__

#define NUMBER_OF_CUSTOMER 50
#define NUMBER_OF_PUMPS 4
#define NUMBER_OF_TANKS 4
#define TANK_SIZE 500
#define MIN_LEVEL 200
#define PUMP_RATE 0.5

#define FUEL82 82
#define FUEL82_PRICE 1.459

#define FUEL87 87
#define FUEL87_PRICE 1.629

#define FUEL92 92
#define FUEL92_PRICE 1.709

#define FUEL97 97
#define FUEL97_PRICE 1.881

#define MAX_AMOUNT 70
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
	time_t purchaseTime;
};


#endif