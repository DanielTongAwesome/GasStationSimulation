#include <stdio.h>
#include "..\rt.h"
#include "GSC.h"

int main() {

	// initial GSC pointer
	GSC *gsc1[1];

	// create each pump and resume
	gsc1[0] = new GSC();
	gsc1[0]->Resume();


	// let the process keep running, otherwise it will be killed
	while (1) {
		continue;
	}

	
	return 0;
}