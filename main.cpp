#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "md5.h"

using namespace std;

int main()
{
    clock_t start_time, end_time;
    double duration;

    start_time = clock();

    md5();
    end_time = clock();

    duration = (end_time - start_time)/CLOCKS_PER_SEC;
    printf("Time : %f sec \n",duration);

    return 0;
}
