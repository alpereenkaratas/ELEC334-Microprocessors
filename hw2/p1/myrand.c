#include <stdio.h>
#include <stdlib.h>
#include "myrand.h"

/*The methods name is Linear Congruential Generator, which i explained in flowchart diagram in the pdf.*/

int JUST_A_NUMBER=23;  // No specific reason associated with the code, it's my hometowns plate number.

int myrand(int lower,int upper){

    JUST_A_NUMBER = ((JUST_A_NUMBER * 8428) + 1) % 78956; // Linear Congruential Generator
    return (JUST_A_NUMBER %(upper-lower+1))+lower;

}
