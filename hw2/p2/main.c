#include <stdio.h>
#include <stdlib.h>
#include "myrand.h"
#include "test_random.h"
#define N 150000            //The number of total generated numbers
#define lower 1             //My lower limit for random number generation
#define upper 15            //My upper limit for random number generation
#define start 1             //Used in printing
#define stop 15             //Used in printing

/*Arrays which i use for keep the values and print their rates.*/
int generated_numbers_array[N];
int how_many_of_each_number[N];
int generated_numbers_array_by_rand[N];     //for rand() func.
int how_many_of_each_number_by_rand[N];     //for rand() func.

int main()
{
    int i=0,k=0;
    int ctr,ctr2=0;
    int idx,idx2=0;
    int m,n;
    int x,y;

/*Here is for myrand() function*/

    while(i<N){
        generated_numbers_array[i]=myrand(lower,upper);  //Numbers generated in myrand function in main.c are thrown into the array.
        i++;
    }

    for(int i=lower;i<=upper;i++){                      //Calculating how many numbers are produced between the specified limits.
        for(int j=0;j<N;j++){
            if(generated_numbers_array[j]==i)
                ctr++;
        }
        how_many_of_each_number[idx]=ctr;              //Calculated values are kept in array one by one.
        idx++;
        ctr=0;

    }

    printf("Problem 2:\n\nresults from myrand():\n\n");     //The calculated values are printed as desired.(Their ratios are calculated.)
    for(m=start,n=0; m<stop,n<stop; m++,n++)
        printf("%d: %.3f    ",m,(float)how_many_of_each_number[n]/N*10);

/*Here is for myrand() function. Almost the same, not much different*/

    while(k<N){
        generated_numbers_array_by_rand[k]=rand_func(lower,upper);   //Numbers generated in rand function in main.c are thrown into the array.
        k++;
    }

    for(int i=lower;i<=upper;i++){                          //Numbers generated in myrand function in main.c are thrown into the array.
        for(int j=0;j<N;j++){
            if(generated_numbers_array_by_rand[j]==i)
                ctr2++;
        }
        how_many_of_each_number_by_rand[idx2]=ctr2;         //Calculated values are kept in array one by one.
        idx2++;
        ctr2=0;
    }

    printf("\n\nresults from rand():\n\n");                 //The calculated values are printed as desired.(Their ratios are calculated.)
    for(x=start,y=0; x<stop,y<stop; x++,y++)
        printf("%d: %.3f    ",x,(float)how_many_of_each_number_by_rand[y]/N*10);

    printf("\n");

    return 0;
}
