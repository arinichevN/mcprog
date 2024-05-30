#include <stdlib.h>
#include "framework/app/interface.h"


int main() {
	app_begin();
	while(1) {
		app_control();
	}
	return EXIT_FAILURE;
}


