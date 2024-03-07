#include "SugiMath.h"

float Radian(float r)
{
    return r * (PI / RAD);
}

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

int32_t Max(int32_t a, int32_t b)
{
    if (a >= b) {
        return a;
    }

    return b;
}

