#include <stdio.h>
#include <stdlib.h>
#include "myrand.h"
#define N 150000       //The number of total generated numbers
#define lower 1        //My lower limit for random number generation
#define upper 15       //My upper limit for random number generation

int generated_numbers_array[N];

int main()
{
    int i=0,k=0;

    while(i<N){
        generated_numbers_array[i]=myrand(lower,upper);     //Numbers generated in myrand function in main.c are thrown into the array.
        i++;
    }

    printf("Problem 1:\n\nA few numbers from myrand():\n");
    for(int a=0;a<3;a++){                                   //Printing the first three numbers of array. Just for verification.
        printf("myrand\n%d\n",generated_numbers_array[a]);
    }
}
