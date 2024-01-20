/**
 * @file SugiMath.h
 * @brief 自作の計算系クラスをまとめたもの
 */

#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4.h"
#include "Easing.h"

const float PI = 3.14159265f;
const float RAD = 180.0f;

//最小を求める
float Min(float a,float b);
float Min(float a,float b ,float c);
float Min(float a, float b, float c, float d);

//最大を求める
float Max(float a, float b);
float Max(float a, float b, float c);
float Max(float a, float b, float c, float d);