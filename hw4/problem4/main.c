/* main.c
*
* Prepared by:
* Alperen Karataş - 1801022022
*
* Notes:
* ELEC334 2020 Fall HW#4 - Problem 4
* State Machines
*
*/

#include "stm32g0xx.h"
#include "bsp.h"

enum modes{
	mode0, mode1, mode2, mode3, mode4
};

enum modes mode;

_Bool button_state=1;
volatile uint32_t sys_count = 0;
int mode_type_cnt=1;

/* interrupt function */
void SysTick_Handler(void) {
     if(sys_count > 0)
    	 sys_count--;
}

/* external interrupt function for button */
void EXTI2_3_IRQHandler(void){

	if ((GPIOB->IDR & (1 << 3)) && button_state == 1) {
		mode_type_cnt++;
		if (mode_type_cnt == 5)
			mode_type_cnt = 0;
		choose_mode();
		button_state = 0;			// for debouncing
	}
	else
		delay_ms(1000000);

	EXTI->RPR1 |= (1U << 3); 	// rising edge pending register

}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	BSP_led_toggle();
	button_state = 1;
	TIM1->SR &= ~(1U << 0);
}

/* this function chooses the state */
void choose_mode(){

	if (mode_type_cnt == mode0)
		TIM1->ARR = 0;
	else if (mode_type_cnt == mode1)
		TIM1->ARR = 1600*10;
	else if (mode_type_cnt == mode2)
		TIM1->ARR = 1600*5;
	else if (mode_type_cnt == mode3)
		TIM1->ARR = 1600*1;
	else if (mode_type_cnt == mode4)
		TIM1->ARR = 1600*39;

	button_state = 1;
}

void delay_ms(volatile uint32_t s) {
	for (int i = 0; i < s; ++i);
}

void init_timer1(){

	/* enable TIM1 module clock */
	RCC->APBENR2 |= (1U << 11);

	TIM1->CR1 = 0;         // make zero the control register just in case
	TIM1->CR1 |= (1 << 7); // ARPE register
	TIM1->CNT = 0;		   // make zero the counter

	/* 1 second interrupt -for first toggle obviously- */
	TIM1->PSC = 999;
	TIM1->ARR = 1600*10;

	TIM1->DIER |= (1 << 0);		// update interrupt enable
	TIM1->CR1 |= (1 << 0);		// TIM1 enable

	/* enable NVIC and set interrupt priority  */
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 3);
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

int main(void) {

	SysTick_Config(16000);
	BSP_led_init();
	init_timer1();
    BSP_button_interrupt_init();
    choose_mode();

    while(1) {

    }

    return 0;
}
