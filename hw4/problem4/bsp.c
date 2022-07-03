/* bsp.c
*
* Prepared by:
* Alperen Karataş - 1801022022
*
*/

#include "stm32g0xx.h"
#include "bsp.h"

void BSP_led_init(void){
	/* Enable GPIOA clock */
	RCC->IOPENR |= (1U << 0);

	/* Setup PA6 as output */
	GPIOA->MODER &= ~(3U << 2*6);
	GPIOA->MODER |= (1U << 2*6);

	/* clear LED */
	GPIOA->BRR |= (1U << 6);
}

void BSP_led_toggle(void){
	GPIOA->ODR ^= (1U << 6);
}

void BSP_led_set(){
	/* Turn on LED on PA6 */
	GPIOA->ODR |= (1U << 6);
}

void BSP_led_clear(){
	/* Turn on LED on PA6 */
	GPIOA->BRR |= (1U << 6);
}

void BSP_button_interrupt_init(void){
	/* Enable GPIOB clock */
	RCC->IOPENR |= (1U << 1);

	/* Setup PB3 as input */
	GPIOB->MODER &= ~(3U << 2*3);

	/* rising edge trigger selection, selection register and mask register */
	EXTI->RTSR1 |= (1U << 3);
	EXTI->EXTICR[0] |= (1U << 8*3);
	EXTI->IMR1 |= (1U << 3);

	/* enable NVIC and set interrupt priority */
	NVIC_SetPriority(EXTI2_3_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

}

void BSP_clearSSD(void){

	GPIOB ->ODR |= (1U << 0);  //PB0 A
	GPIOB ->ODR |= (1U << 1);  //PB1 B
	GPIOB ->ODR |= (1U << 2);  //PB2 C
	GPIOB ->ODR |= (1U << 3);  //PB3 D
	GPIOB ->ODR |= (1U << 4);  //PB4 E
	GPIOB ->ODR |= (1U << 5);  //PB5 F
	GPIOB ->ODR |= (1U << 6);  //PB6 G
}

void BSP_setSSD(int x){
	BSP_clearSSD();
	switch(x){
		case 0:
			GPIOB->ODR &= ~(1U << 0); //PB0 A
			GPIOB->ODR &= ~(1U << 1); //PB1 B
			GPIOB->ODR &= ~(1U << 2); //PB2 C
			GPIOB->ODR &= ~(1U << 3); //PB3 D
			GPIOB->ODR &= ~(1U << 4); //PB4 E
			GPIOB->ODR &= ~(1U << 5); //PB5 F
            break;

		case 1:
			GPIOB->ODR &= ~(1U << 1); //PB1 B
			GPIOB->ODR &= ~(1U << 2); //PB2 C
            break;

		case 2:
			GPIOB->ODR &= ~(1U << 0);  //PB0 A
			GPIOB->ODR &= ~(1U << 1);  //PB1 B
			GPIOB->ODR &= ~(1U << 3);  //PB3 D
			GPIOB->ODR &= ~(1U << 4); //PB4 E
			GPIOB->ODR &= ~(1U << 6);  //PB6 G
            break;

		case 3:
			GPIOB->ODR &= ~(1U << 0); //PB0 A
			GPIOB->ODR &= ~(1U << 1); //PB1 B
			GPIOB->ODR &= ~(1U << 2); //PB2 C
			GPIOB->ODR &= ~(1U << 3); //PB3 D
			GPIOB->ODR &= ~(1U << 6);  //PB6 G
            break;

		case 4:
			GPIOB->ODR &= ~(1U << 1); //PB1 B
			GPIOB->ODR &= ~(1U << 2); //PB2 C
			GPIOB->ODR &= ~(1U << 5); //PB5 F
			GPIOB->ODR &= ~(1U << 6);  //PB6 G
            break;

		case 5:
			GPIOB->ODR &= ~(1U << 0); //PB0 A
			GPIOB->ODR &= ~(1U << 2); //PB2 C
			GPIOB->ODR &= ~(1U << 3); //PB3 D
			GPIOB->ODR &= ~(1U << 5); //PB5 F
			GPIOB->ODR &= ~(1U << 6);  //PB6 G
            break;

		case 6:
			GPIOB->ODR &= ~(1U << 0); //PB0 A
			GPIOB->ODR &= ~(1U << 2); //PB2 C
			GPIOB->ODR &= ~(1U << 3); //PB3 D
			GPIOB->ODR &= ~(1U << 4); //PB4 E
			GPIOB->ODR &= ~(1U << 5); //PB5 F
			GPIOB->ODR &= ~(1U << 6);  //PB6 G
            break;

		case 7:
			GPIOB->ODR &= ~(1U << 0); //PB0 A
			GPIOB->ODR &= ~(1U << 1); //PB1 B
			GPIOB->ODR &= ~(1U << 2); //PB2 C
            break;

		case 8:
			GPIOB->ODR &= ~(1U << 0); //PA0 A
			GPIOB->ODR &= ~(1U << 1); //PB1 B
			GPIOB->ODR &= ~(1U << 2); //PB2 C
			GPIOB->ODR &= ~(1U << 3); //PB3 D
			GPIOB->ODR &= ~(1U << 4); //PB4 E
			GPIOB->ODR &= ~(1U << 5); //PB5 F
			GPIOB->ODR &= ~(1U << 6);  //PB6 G
            break;

		case 9:
			GPIOB->ODR &= ~(1U << 0); //PB0 A
			GPIOB->ODR &= ~(1U << 1); //PB1 B
			GPIOB->ODR &= ~(1U << 2); //PB2 C
			GPIOB->ODR &= ~(1U << 3); //PB3 D
			GPIOB->ODR &= ~(1U << 5); //PB5 F
			GPIOB->ODR &= ~(1U << 6);  //PB6 G
            break;
	}
}
