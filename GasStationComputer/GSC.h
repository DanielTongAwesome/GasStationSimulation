#ifndef __GSC__
#define __GSC__

#include "..\rt.h"
#include <stdio.h>
#include "..\GasStationSimulation\\constants.h"

// GAS STATION COMPUTER class
// Active class, we prefer not to set up many things in the class declearation
// check gsc.cpp to see more detailed information of its running
// gsc.cpp is the hardest part in our structure
class GSC : public ActiveClass {

private:
	
public:

	GSC();  // constructor
	~GSC();	// destuctor


private:
	// main logic here
	int main(void);
};

#endif