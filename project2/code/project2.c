/* main.c
*
* Prepared by:
* Alperen Karatas - 1801022022
*
* Notes:
* ELEC334 2020 Fall - Project 2
* Fully Operational Scientific Calculator
*
*/

#include "stm32g0xx.h"
#include "math.h"
#include "bsp.h"

int main(void) {

	moders_for_digits();
	SSD_activate();
	keypad_read();
	school_id();
	clearSSD();

    while(1) {

    }

    return 0;
}
