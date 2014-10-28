#include <string.h>
#include "data.h"

data::data(int len, char* w)
{
    //ctor


    length = len;
    strcpy(word,w);
}

data::~data()
{
    //dtor
}
