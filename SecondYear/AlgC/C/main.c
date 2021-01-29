#include <stdio.h>
#include "gui.h"

int main (void) {
	int option = mainProgram();

	switch (option) {
		case 1: GUIavlTree();
				break;

		case 2: GUIminHeaps();
				break;

		case 3: GUIhashTables();
				break;

		default: break;
	}

	return 0;
}
