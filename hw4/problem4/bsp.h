/* bsp.h
*
* Prepared by:
* Alperen Karataş - 1801022022
*
*/

#ifndef BSP_H_
#define BSP_H_

void BSP_led_init(void);
void BSP_led_toggle(void);
void BSP_led_set();
void BSP_led_clear();
void BSP_button_interrupt_init(void);
void BSP_clearSSD(void);
void BSP_setSSD(int x);

#endif
