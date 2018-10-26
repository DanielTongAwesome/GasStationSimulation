#include <stdio.h>
#include "..\rt.h"
#include "GSC.h"

int main() {

	// initial GSC pointer
	GSC *gsc1[1];

	// create each pump and resume
	
	gsc1[0] = new GSC();
	gsc1[0]->Resume();
	


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