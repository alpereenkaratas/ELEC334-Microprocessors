#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_random.h"

int rand_func(int lower,int upper){

    return (rand() %(upper-lower+1))+lower;   //Simple rand function, as we already know.

}
