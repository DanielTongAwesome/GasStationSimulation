#include <stdio.h>
#include "..\rt.h"
#include "Pump.h"

int main() {
	
	Pump *pumps[NUMBER_OF_PUMPS];

	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		pumps[i] = new Pump(i + 1);
		pumps[i]->Resume();
	}
	

	while (1) {
		continue;
	}

	/*Test code for customer pipeline*/
	/*Pump *pump[1];
	pump[1] = new Pump(1);
	pump[1]->Resume();
*/

	//system("PAUSE");
	return 0;
}