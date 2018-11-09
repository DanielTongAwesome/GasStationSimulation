#include <stdio.h>
#include "..\rt.h"
#include "Pump.h"

int main() {
	
	// initial pump pointer
	Pump *pumps[NUMBER_OF_PUMPS];

	// create each pump and resume
	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		// assign pump ID
		pumps[i] = new Pump(i + 1);
		pumps[i]->Resume();
	}
	
	// let it keep running, otherwise this process will be killed
	while (1) {
		continue;
	}

	
	return 0;
}