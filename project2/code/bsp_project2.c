/* bsp.c
*
* Prepared by:
* Alperen Karataş - 1801022022
*
*/
 _Bool button_state = 1;

volatile int number=0;
volatile int number_catch=0;
int control=0;

volatile int is_in_add=0;
volatile int is_in_ext=0;
volatile int is_in_mul=0;
volatile int is_in_div=0;

volatile int is_in_sci=0;
volatile int is_in_sci_log10=0;
volatile int is_in_sci_ln=0;
volatile int is_in_sci_root=0;
volatile int is_in_sci_square=0;

volatile int is_in_tri=-1;	// attention, it's initial value is -1 because of double pressing for trigonometric mode
volatile int is_in_tri_sin=0;
volatile int is_in_tri_cos=0;
volatile int is_in_tri_tan=0;
volatile int is_in_tri_cot=0;
volatile float is_in_tri_pi=0;

volatile int final=0;
volatile float final_sci_tri=0.0;

#include "stm32g0xx.h"
#include "bsp.h"

static int count =0;

void SysTick_Handler(void){
	count++;
}

void delay_ms(volatile uint32_t s) {
    count = 0;
    while(count < s);
}

void keypad_read(void){
		// Enable GPIOA clock
		    RCC->IOPENR |= (1U << 0);

		// Enable GPIOB clock
		    RCC->IOPENR |= (1U << 1);

		// Setup pa8, pb9, pb5, pb4 as output rows
		    GPIOA->MODER &= ~(3U << 2*8);
		    GPIOA->MODER |= (1U << 2*8);

		    GPIOB->MODER &= ~(3U << 2*9);
		    GPIOB->MODER |= (1U << 2*9);

		    GPIOB->MODER &= ~(3U << 2*5);
		    GPIOB->MODER |= (1U << 2*5);

		    GPIOB->MODER &= ~(3U << 2*4);
		    GPIOB->MODER |= (1U << 2*4);

		//Setup pa9, pb0, pb2, pb8 as input columns
		    GPIOA->MODER &= ~(3U << 2*9);
		    GPIOA->PUPDR |= (2U << 2*9);

		    GPIOB->MODER &= ~(3U << 0);
		    GPIOB->PUPDR |= (2U << 0);

		    GPIOB->MODER &= ~(3U << 2*2);
		    GPIOB->PUPDR |= (2U << 2*2);

		    GPIOB->MODER &= ~(3U << 2*8);
		    GPIOB->PUPDR |= (2U << 2*8);

		// Setup interrupts for pa9, pb0,pb2,pb8
		    EXTI->EXTICR[2] |= (0U << 8*1); //pa9
		    EXTI->EXTICR[0] |= (1U << 0);   //pb0
		    EXTI->EXTICR[0] |= (1U << 8*2); //pb2
		    EXTI->EXTICR[2] |= (1U << 0);   //pb8

		//rising edge selected for pa9, pb0,pb2,pb8
		    EXTI->RTSR1 |= (1U << 9);
		    EXTI->RTSR1 |= (1U << 0);
		    EXTI->RTSR1 |= (1U << 2);
		    EXTI->RTSR1 |= (1U << 8);

		//mask register selected for pa9, pb0,pb2,pb8
		    EXTI->IMR1 |= (1U << 9);
		    EXTI->IMR1 |= (1U << 0);
		    EXTI->IMR1 |= (1U << 2);
		    EXTI->IMR1 |= (1U << 8);

		// enable NVIC and set interrupt priority
		    NVIC_SetPriority(EXTI0_1_IRQn, 0);
		    NVIC_EnableIRQ(EXTI0_1_IRQn);

		    NVIC_SetPriority(EXTI2_3_IRQn, 0);
		    NVIC_EnableIRQ(EXTI2_3_IRQn);

		    NVIC_SetPriority(EXTI4_15_IRQn, 0);
		    NVIC_EnableIRQ(EXTI4_15_IRQn);

			/* Setup PA0 as output */
			GPIOA->MODER &= ~(3U << 2 * 0);
			GPIOA->MODER |= (1U << 2 * 0);
			/* Setup PA1 as output */
			GPIOA->MODER &= ~(3U << 2 * 1);
			GPIOA->MODER |= (1U << 2 * 1);
			/* Setup PA4 as output */
			GPIOA->MODER &= ~(3U << 2 * 4);
			GPIOA->MODER |= (1U << 2 * 4);
			/* Setup PA5 as output */
			GPIOA->MODER &= ~(3U << 2 * 5);
			GPIOA->MODER |= (1U << 2 * 5);
			/* Setup PA6 as output */
			GPIOA->MODER &= ~(3U << 2 * 6);
			GPIOA->MODER |= (1U << 2 * 6);
			/* Setup PA11 as output */
			GPIOA->MODER &= ~(3U << 2 * 11);
			GPIOA->MODER |= (1U << 2 * 11);
			/* Setup PA12 as output */
			GPIOA->MODER &= ~(3U << 2 * 12);
			GPIOA->MODER |= (1U << 2 * 12);



			GPIOA -> ODR |= (1U << 8);  //PA0 A
			GPIOB -> ODR |= (1U << 9);  //PA1 B
			GPIOB -> ODR |= (1U << 5);  //PA4 C
			GPIOB -> ODR |= (1U << 4);  //PA5 D

			clearSSD();
}

void EXTI0_1_IRQHandler(void){ //interrupt from pb0
	clear_Rows_Keypad();

	GPIOA->ODR ^= (1U << 8);
	if ((GPIOB -> IDR >>0)& 1){
		setSSD(15); //#
	}
	GPIOA->ODR ^= (1U << 8); //pa8


	GPIOB->ODR ^= (1U << 9); //pb9
	if ((GPIOB -> IDR >>0)& 1){
			setSSD(9);
			number=number*pow(10,control);
			number+=9;
			control=1;
		}
	GPIOB->ODR ^= (1U << 9); //pb9


	GPIOB->ODR ^= (1U << 5); //pb5
		if ((GPIOB -> IDR >>0)& 1){
				setSSD(6);
				number=number*pow(10,control);
				number+=6;
				control=1;
			}
	GPIOB->ODR ^= (1U << 5); //pb5


	GPIOB->ODR ^= (1U << 4); //pb4
		if ((GPIOB -> IDR >>0)& 1){
				setSSD(3);
				number=number*pow(10,control);
				number+=3;
				control=1;
			}
	GPIOB->ODR ^= (1U << 4); //pb4

	EXTI->RPR1 |= (1U << 0); //clear interrupt flag
	set_Rows_Keypad();

}

void EXTI2_3_IRQHandler(void){ //interrupt from PB2
	clear_Rows_Keypad();
	GPIOA->ODR ^= (1U << 8);
	if((GPIOB->IDR >>2) & 1){
		setSSD(0);
		number=number*pow(10,control);
		number+=0;
		control=1;
	}
	GPIOA->ODR ^= (1U << 8); //pa8


	GPIOB->ODR ^= (1U << 9); //pb9
	if((GPIOB->IDR >>2) & 1){
		setSSD(8);
		number=number*pow(10,control);
		number+=8;
		control=1;
	}
	GPIOB->ODR ^= (1U << 9); //pb9


	GPIOB->ODR ^= (1U << 5); //pb5
	if((GPIOB->IDR >>2) & 1){
		setSSD(5);
		number=number*pow(10,control);
		number+=5;
		control=1;
	}
	GPIOB->ODR ^= (1U << 5); //pb5


	GPIOB->ODR ^= (1U << 4); //pb4
	if((GPIOB->IDR >>2) &1){
		setSSD(2);
		number=number*pow(10,control);
		number+=2;
		control=1;
	}
	GPIOB->ODR ^= (1U << 4); //pb4

	EXTI->RPR1 |= (1U << 2); //clear interrupt flag
	set_Rows_Keypad();
}

void EXTI4_15_IRQHandler(void){ //pb8 and pa9
	if((GPIOB->IDR >>8) &1){

		clear_Rows_Keypad();
		GPIOA->ODR ^= (1U << 8);
		if((GPIOB->IDR >>8) &1){
			setSSD(14);

		}
		GPIOA->ODR ^= (1U << 8); //pa8


		GPIOB->ODR ^= (1U << 9); //pb9
		if((GPIOB->IDR >>8) &1){
			setSSD(7);
			number=number*pow(10,control);
			number+=7;
			control=1;
		}
		GPIOB->ODR ^= (1U << 9); //pb9


		GPIOB->ODR ^= (1U << 5); //pb5
		if((GPIOB->IDR >>8) &1){
			setSSD(4);
			number=number*pow(10,control);
			number+=4;
			control=1;
		}
		GPIOB->ODR ^= (1U << 5); //pb5


		GPIOB->ODR ^= (1U << 4); //pb4
		if((GPIOB->IDR >>8) &1){
			setSSD(1);
			number=number*pow(10,control);
			number+=1;
			control=1;
		}
		GPIOB->ODR ^= (1U << 4); //pb4
		EXTI->RPR1 |= (1U << 8); //clear interrupt flag
		set_Rows_Keypad();
	}

	if((GPIOA->IDR >>9) &1){
		clear_Rows_Keypad();
		GPIOA->ODR ^= (1U << 8);
		if((GPIOA->IDR >>9) &1){
			setSSD(13);
		}
		GPIOA->ODR ^= (1U << 8); //pa8


		GPIOB->ODR ^= (1U << 9); //pb9
		if((GPIOA->IDR >>9) &1){
			setSSD(12);
		}
		GPIOB->ODR ^= (1U << 9); //pb9


		GPIOB->ODR ^= (1U << 5); //pb5
		if((GPIOA->IDR >>9) &1){
			setSSD(11);
		}
		GPIOB->ODR ^= (1U << 5); //pb5


		GPIOB->ODR ^= (1U << 4); //pb4
		if((GPIOA->IDR >>9) &1){
			setSSD(10);
		}
		GPIOB->ODR ^= (1U << 4); //pb4
		EXTI->RPR1 |= (1U << 9); //clear interrupt flag
		set_Rows_Keypad();
	}
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

void clearSSD(void){

	GPIOA -> ODR |= (1U << 0);  //PA0 A
	GPIOA -> ODR |= (1U << 1);  //PA1 B
	GPIOA -> ODR |= (1U << 4);  //PA4 C
	GPIOA -> ODR |= (1U << 5);  //PA5 D
	GPIOA -> ODR |= (1U << 12); //PA12 E
	GPIOA -> ODR |= (1U << 11); //PA11 F
	GPIOA -> ODR |= (1U << 6);  //PA6 G
}


void setSSD(int x){
	clearSSD();
	switch(x){
		case 0:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA5 D
			GPIOA -> ODR &= ~(1U << 12); //PA12 E
			GPIOA -> ODR &= ~(1U << 11); //PA11 F
			break;

		case 1:
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			break;

		case 2:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 5);  //PA5 D
			GPIOA -> ODR &= ~(1U << 12); //PA12 E
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;

		case 3:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA5 D
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;
		case 4:
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 11); //PA11 F
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;

		case 5:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA5 D
			GPIOA -> ODR &= ~(1U << 11); //PA11 F
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;

		case 6:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA5 D
			GPIOA -> ODR &= ~(1U << 12); //PA12 E
			GPIOA -> ODR &= ~(1U << 11); //PA11 F
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;

		case 7:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			break;

		case 8:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA5 D
			GPIOA -> ODR &= ~(1U << 12); //PA12 E
			GPIOA -> ODR &= ~(1U << 11); //PA11 F
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;

		case 9:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA5 D
			GPIOA -> ODR &= ~(1U << 11); //PA11 F
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
            break;

        /* A -> addition - logarithm base 10 - sin */
		case 10:
			if(number!=0 && is_in_sci==0 && is_in_tri==-1){
				number &= 0x7FFFFFFF;
				number_catch = number;
				is_in_add++;
				number = 0;
			}
			if(is_in_tri==1){
				number &= 0x7FFFFFFF;
				is_in_tri_sin++;
			}
			else if(is_in_sci==1){
				number &= 0x7FFFFFFF;
				is_in_sci_log10++;
			}
			break;

		/* B -> extraction - natural logarithm (ln) - cos */
		case 11:
			if(number!=0 && is_in_sci==0 && is_in_tri==-1){
				number &= 0x7FFFFFFF;
				number_catch = number;
				is_in_ext++;
				number = 0;
			}
			if(is_in_tri==1){
				number &= 0x7FFFFFFF;
				is_in_tri_cos++;
			}
			else if(is_in_sci==1){
				number &= 0x7FFFFFFF;
				is_in_sci_ln++;
			}
			break;

		/* C -> multiplication - square root - tan */
		case 12:
			if(number!=0 && is_in_sci==0 && is_in_tri==-1){
				number &= 0x7FFFFFFF;
				number_catch = number;
				is_in_mul++;
				number = 0;
			}
			if(is_in_tri==1){
				number &= 0x7FFFFFFF;
				is_in_tri_tan++;
			}
			else if(is_in_sci==1){
				number &= 0x7FFFFFFF;
				is_in_sci_root++;
			}
			break;

		/* D -> division - square - cot */
		case 13:
			if(number!=0 && is_in_sci==0 && is_in_tri==-1){
				number &= 0x7FFFFFFF;
				number_catch = number;
				is_in_div++;
				number = 0;

			}
			if(is_in_tri==1){
				number &= 0x7FFFFFFF;
				is_in_tri_cot++;
			}

			else if(is_in_sci==1){
				number &= 0x7FFFFFFF;
				is_in_sci_square++;
			}
			break;

		/* * -> final results */
		case 14:
			if(is_in_add==1){
				if (number!=0)
					number &= 0x7FFFFFFF;
				final = number_catch+number;
				print_final(final);
			}
			if(is_in_ext==1){
				if (number!=0)
					number &= 0x7FFFFFFF;
				final = number_catch-number;
				print_final(final);
			}
			if(is_in_mul==1){
				if (number!=0)
					number &= 0x7FFFFFFF;
				final = number_catch*number;
				if(final < -999 || final > 9999){   	// overflow
					overflow_inputs();
				}
				else{
					print_final(final);
				}

			}
			if(is_in_div==1){
				if (number!=0)
					number &= 0x7FFFFFFF;
				final = number_catch/number;
				if(number==0){
					invalid_inputs();
				}
				else{
					print_final(final);
				}
			}
			if(is_in_sci_log10==1){
				final_sci_tri=log10(number);
				final=final_sci_tri*1000;
				print_final_dot(final);
			}
			if(is_in_sci_ln==1){
				final_sci_tri=log(number);
				final=final_sci_tri*1000;
				print_final_dot(final);
			}
			if(is_in_sci_root==1){
				final_sci_tri=sqrt(number);
				if(number<0){					// invalid
					invalid_inputs();
				}
				else{
					final=final_sci_tri*1000;
					print_final(final);
				}
			}
			if(is_in_sci_square==1){
				final=pow(number,2);
				print_final(final);
			}
			if(is_in_tri_sin==1){
				final_sci_tri=sin(number);
				final=final_sci_tri*1000;
				print_final_dot(final);
			}
			if(is_in_tri_cos==1){
				final_sci_tri=cos(number);
				final=final_sci_tri*1000;
				print_final_dot(final);
			}
			if(is_in_tri_tan==1){
				final_sci_tri=tan(number);
				final=final_sci_tri*1000;
				print_final_dot(final);
			}
			if(is_in_tri_cot==1){
				final_sci_tri=1/tan(number);
				final=final_sci_tri*1000;
				print_final_dot(final);
			}
			break;

		/* # -> scientific - if pressed three times it will show the number of pi */
		case 15:
			if(is_in_tri==1){
				final=3141;			// pi=3.141
				print_final_pi(final);
			}
			is_in_sci++;
			is_in_tri++;
			break;

		/* O */
		case 16:
			GPIOA -> ODR &= ~(1U << 4);  //PA2 C
			GPIOA -> ODR &= ~(1U << 5);  //PA3 D
			GPIOA -> ODR &= ~(1U << 12); //PA4 E
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;

		/* u */
		case 17:
			GPIOA -> ODR &= ~(1U << 4);  //PA2 C
			GPIOA -> ODR &= ~(1U << 5);  //PA3 D
			GPIOA -> ODR &= ~(1U << 12); //PA4 E
			break;

		/* f */
		case 18:
			GPIOA -> ODR &= ~(1U << 0);  //PA0 A
			GPIOA -> ODR &= ~(1U << 12); //PA4 E
			GPIOA -> ODR &= ~(1U << 11); //PA5 F
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			break;

		/* L */
		case 19:
			GPIOA -> ODR &= ~(1U << 5);  //PA3 D
			GPIOA -> ODR &= ~(1U << 12); //PA4 E
			GPIOA -> ODR &= ~(1U << 11); //PA5 F
			break;

		/* I */
		case 20:
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			break;

		/* n */
		case 21:
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			GPIOA -> ODR &= ~(1U << 12); //PA12 E
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			break;

		/* u */
		case 22:
			GPIOA -> ODR &= ~(1U << 12); //PA12 E
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA3 D
			break;

		/* d */
		case 23:
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			GPIOA -> ODR &= ~(1U << 12); //PA12 E
			GPIOA -> ODR &= ~(1U << 4);  //PA4 C
			GPIOA -> ODR &= ~(1U << 5);  //PA3 D
			GPIOA -> ODR &= ~(1U << 1);  //PA1 B
			break;

		/* minus(-) sign */
		case 24:
			odr_for_digit1();
			moder_deactivate_dot();
			GPIOA -> ODR &= ~(1U << 6);  //PA6 G
			delay(500);
			break;

		/* decimal point (dot) */
		case 25:
			GPIOA -> ODR &= ~(1U << 7);  //PA7 DP
			delay(500);
			break;
	}
}

/* addition, extraction, multiplication and division print function for SSD */
void print_final(int t){
	if(t<0){
		t=t*(-1);
		int x=t/1000;
		int y=(t/100)%10;
		int z=(t/10)%10;
		int k=t%10;
		for(int i=0; i<6500; ++i){
			setSSD(24);
			digit2_SSD(x);
			digit3_SSD(y);
			digit4_SSD(z);
		}
		school_id();        // back to idle state
	}
	 else{
		 int x=t/1000;
		 int y=(t/100)%10;
		 int z=(t/10)%10;
		 int k=t%10;
		 for(int i=0; i<6500; ++i){
			digit1_SSD(x);
			digit2_SSD(y);
			digit3_SSD(z);
			digit4_SSD(k);
		}
		 school_id();        // back to idle state
	}
}

/* scintific/trigonometric (float numbers) print function for SSD */
void print_final_dot(int t){
	if(t<0){
		t=t*(-1);
		int x=t/1000;
		int y=(t/100)%10;
		int z=(t/10)%10;
		int k=t%10;
		if(t>0 && t<10000){
			for(int i=0; i<6500; ++i){
				odr_for_digit2();
				moder_activate_dot();
				setSSD(25);
				setSSD(24);
				digit2_SSD(x);
				digit3_SSD(y);
				digit4_SSD(z);
			}
			school_id();        // back to idle state
		}
		else if(t>=10000 && t<=100000){
			for(int i=0; i<6500; ++i){
				odr_for_digit3();
				moder_activate_dot();
				setSSD(25);
				setSSD(24);
				digit2_SSD(x);
				digit3_SSD(y);
				digit4_SSD(z);
			}
		}
		school_id();        // back to idle state
	}
	 else{
		 int x=t/1000;
		 int y=(t/100)%10;
		 int z=(t/10)%10;
		 int k=t%10;
		 if(t>0 && t<10000){
			 for(int i=0; i<6500; ++i){
				 odr_for_digit1();
				 moder_activate_dot();
				 setSSD(25);
				 digit1_SSD(x);
				 digit2_SSD(y);
				 digit3_SSD(z);
				 digit4_SSD(k);
			 }
			 school_id();        // back to idle state
		 }
		 else if(t>=10000 && t<=100000){
			 for(int i=0; i<6500; ++i){
				 odr_for_digit2();
				 moder_activate_dot();
				 setSSD(25);
				 digit1_SSD(x);
				 digit2_SSD(y);
				 digit3_SSD(z);
				 digit4_SSD(k);
			 }
			 school_id();        // back to idle state
		 }
		 else if(t>=10000 && t<=100000){
			 for(int i=0; i<6500; ++i){
				 odr_for_digit3();
				 moder_activate_dot();
				 setSSD(25);
				 digit1_SSD(x);
				 digit2_SSD(y);
				 digit3_SSD(z);
				 digit4_SSD(k);
			 }
		 }
		 school_id();		// back to idle state
	}
}

/* pi number print function for SSD */
void print_final_pi(int t){
		 int x=t/1000;
		 int y=(t/100)%10;
		 int z=(t/10)%10;
		 int k=t%10;
		 for(int i=0; i<6500; ++i){
			odr_for_digit1();
			moder_activate_dot();
			digit1_SSD(x);
			digit2_SSD(y);
			digit3_SSD(z);
			digit4_SSD(k);
		 }
		 school_id();        // back to idle state
}

void invalid_inputs(){
int ctr_inv=0;
while(ctr_inv<10000){
	/* I */
	odr_for_digit1();
	setSSD(20);
	delay(500);
	/* n */
	odr_for_digit2();
	setSSD(21);
	delay(500);
	/* u */
	odr_for_digit3();
	setSSD(22);
	delay(500);
	/* d */
	odr_for_digit4();
	setSSD(23);
	delay(500);
	ctr_inv++;
}
	clearSSD();
}

void overflow_inputs(){
int ctr_ovr=0;
while(ctr_ovr<10000){
	/* O */
	odr_for_digit1();
	setSSD(16);
	delay(500);
	/* u */
	odr_for_digit2();
	setSSD(17);
	delay(500);
	/* F */
	odr_for_digit3();
	setSSD(18);
	delay(500);
	/* L */
	odr_for_digit4();
	setSSD(19);
	delay(500);
	ctr_ovr++;
}
	clearSSD();

}

void clear_Rows_Keypad(void){
	GPIOA -> ODR &= ~(1U << 8);  //PA8
	GPIOB -> ODR &= ~(1U << 9);  //PB9
	GPIOB -> ODR &= ~(1U << 5);  //PB5
	GPIOB -> ODR &= ~(1U << 4);  //PB4
}

void set_Rows_Keypad(void){
	GPIOA -> ODR |= (1U << 8);  //PA8
	GPIOB -> ODR |= (1U << 9);  //PB9
	GPIOB -> ODR |= (1U << 5);  //PB5
	GPIOB -> ODR |= (1U << 4);  //PB4
}

void moders_for_digits(){
	RCC->IOPENR |= (1U << 1);
	GPIOB->MODER &= ~(3U << 2*1);
	GPIOB->MODER |= (1U << 2*1);
	GPIOB->MODER &= ~(3U << 2*3);
	GPIOB->MODER |= (1U << 2*3);
	GPIOB->MODER &= ~(3U << 2*6);
	GPIOB->MODER |= (1U << 2*6);
	GPIOB->MODER &= ~(3U << 2*7);
	GPIOB->MODER |= (1U << 2*7);
}

void school_id(){
int ctr_id=0;
while(ctr_id<5000){
	odr_for_digit1();
	setSSD(1);
	delay(500);
	odr_for_digit2();
	setSSD(8);
	delay(500);
	odr_for_digit3();
	setSSD(2);
	delay(500);
	odr_for_digit4();
	setSSD(2);
	delay(500);
	ctr_id++;
}
clearSSD();
}

void odr_for_digit1(){
	GPIOB ->ODR |= (1U << 6);   // D1  digit to PB6
	GPIOB ->ODR &= ~(1U << 7);  // D2  digit to PB7
	GPIOB ->ODR &= ~(1U << 1);  // D3  digit to PB1
	GPIOB ->ODR &= ~(1U << 3);  // D4  digit to PB3
}
void odr_for_digit2(){
	GPIOB ->ODR &= ~(1U << 6);
	GPIOB ->ODR |= (1U << 7);
	GPIOB ->ODR &= ~(1U << 1);
	GPIOB ->ODR &= ~(1U << 3);
}
void odr_for_digit3(){
	GPIOB ->ODR &= ~(1U << 6);
	GPIOB ->ODR &= ~(1U << 7);
	GPIOB ->ODR |= (1U << 1);
	GPIOB ->ODR &= ~(1U << 3);
}
void odr_for_digit4(){
	GPIOB ->ODR &= ~(1U << 6);
	GPIOB ->ODR &= ~(1U << 7);
	GPIOB ->ODR &= ~(1U << 1);
	GPIOB ->ODR |= (1U << 3);
}

void SSD_activate(void){
	GPIOB ->ODR |= (1U << 6);
	GPIOB ->ODR |= (1U << 7);
	GPIOB ->ODR |= (1U << 1);
	GPIOB ->ODR |= (1U << 3);
}

void digit1_SSD(int x){
	GPIOB ->ODR |=   (1U << 6);
	GPIOB ->ODR &=  ~(1U << 7);
	GPIOB ->ODR &=  ~(1U << 1);
	GPIOB ->ODR &=  ~(1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}
void digit2_SSD(int x){
	moder_deactivate_dot();
	GPIOB ->ODR &=  ~(1U << 6);
	GPIOB ->ODR |=   (1U << 7);
	GPIOB ->ODR &=  ~(1U << 1);
	GPIOB ->ODR &=  ~(1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}
void digit3_SSD(int x){
	moder_deactivate_dot();
	GPIOB ->ODR &=  ~(1U << 6);
	GPIOB ->ODR &=  ~(1U << 7);
	GPIOB ->ODR |=   (1U << 1);
	GPIOB ->ODR &=  ~(1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}

void digit4_SSD(int x){
	moder_deactivate_dot();
	GPIOB ->ODR &=  ~(1U << 6);
	GPIOB ->ODR &=  ~(1U << 7);
	GPIOB ->ODR &=  ~(1U << 1);
	GPIOB ->ODR |=   (1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}

void moder_activate_dot(){
	/* Setup PA7 as output */
	GPIOA->MODER &= ~(3U << 2 * 7);
	GPIOA->MODER |= (1U << 2 * 7);
}

void moder_deactivate_dot(){
	/* Setup PA7 as output */
	GPIOA->MODER &= ~(3U << 2 * 7);
}
