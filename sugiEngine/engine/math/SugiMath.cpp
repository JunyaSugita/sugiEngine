#include "SugiMath.h"

float Min(float a, float b)
{
    if (a < b) {
        return a;
    }

    return b;
}

float Min(float a, float b, float c)
{
    float temp;
    temp = Min(a,b);

    return Min(temp,c);
}

float Min(float a, float b, float c, float d)
{
    float temp;
    temp = Min(a, b, c);

    return Min(temp, d);
}

float Max(float a, float b)
{
    if (a >= b) {
        return a;
    }

    return b;
}

float Max(float a, float b, float c)
{
    float temp;
    temp = Max(a, b);

    return Max(temp, c);
}

float Max(float a, float b, float c, float d)
{
    float temp;
    temp = Max(a, b, c);

    return Max(temp, d);
}

