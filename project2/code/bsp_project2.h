/* bsp.h
*
* Prepared by:
* Alperen Karataş - 1801022022
*
*/

#ifndef BSP_H_
#define BSP_H_

void delay_ms(volatile uint32_t s);
void keypad_read(void);
void delay(volatile uint32_t s);
void clearSSD(void);
void setSSD(int x);
void print_final(int t);
void print_final_dot(int t);
void print_final_pi(int t);
void invalid_inputs();
void overflow_inputs();
void clear_Rows_Keypad(void);
void set_Rows_Keypad(void);
void moders_for_digits();
void school_id();
void odr_for_digit1();
void odr_for_digit2();
void odr_for_digit3();
void odr_for_digit4();
void SSD_activate(void);
void digit1_SSD(int x);
void digit2_SSD(int x);
void digit3_SSD(int x);
void digit4_SSD(int x);
void moder_activate_dot();
void moder_deactivate_dot();
void initTIM3(void);

#endif
