#pragma once
#include "emmintrin.h"
#include "xmmintrin.h"
#include "smmintrin.h"
int IM_GetMirrorPos(int Length, int Pos)
{
	if (Pos < 0)
		return -Pos;
	else if (Pos >= Length)
		return Length + Length - Pos - 2;
	else
		return Pos;
}

void FillLeftAndRight_Mirror_C(int* Array, int Length, int Radius)
{
	for (int X = 0; X < Radius; X++)
	{
		Array[X] = Array[Radius + Radius - X];
		Array[Radius + Length + X] = Array[Radius + Length - X - 2];
	}
}

void FillLeftAndRight_Mirror_SSE(int* Array, int Length, int Radius)
{
	int BlockSize = 2, Block = Radius / BlockSize;
	for (int X = 0; X < Block * BlockSize; X += BlockSize)
	{
		__m128i SrcV1 = _mm_loadu_si128((__m128i*)(Array + Radius + Radius - X - 3));
		__m128i SrcV2 = _mm_loadu_si128((__m128i*)(Array + Radius + Length - X - 5));
		_mm_storeu_si128((__m128i*)(Array + X), _mm_shuffle_epi32(SrcV1, _MM_SHUFFLE(0, 1, 2, 3)));
		_mm_storeu_si128((__m128i*)(Array + Radius + Length + X), _mm_shuffle_epi32(SrcV2, _MM_SHUFFLE(0, 1, 2, 3)));
	}
	//　　处理剩余不能被SSE优化的数据
}

int SumofArray_C(int* Array, int Length)
{
	int Sum = 0;
	for (int X = 0; X < Length; X++)
	{
		Sum += Array[X];
	}
	return Sum;
}
int SumI32(__m128i Src)
{
	__m128i T = _mm_packs_epi32(Src, Src);
	T = _mm_packus_epi16(T, T);
	return _mm_cvtsi128_si32(T);
}
int SumofArray_SSE(int* Array, int Length)
{
	int BlockSize = 2, Block = Length / BlockSize;
	__m128i Sum1 = _mm_setzero_si128();
	__m128i Sum2 = _mm_setzero_si128();
	for (int X = 0; X < Block * BlockSize; X += BlockSize)
	{
		Sum1 = _mm_add_epi32(Sum1, _mm_loadu_si128((__m128i*)(Array + X + 0)));
		Sum2 = _mm_add_epi32(Sum2, _mm_loadu_si128((__m128i*)(Array + X + 4)));
	}
	__m128i z = _mm_add_epi32(Sum1, Sum2);
	int Sum = _mm_cvtsi128_si32(_mm_add_epi32(Sum1, Sum2));
	//　　处理剩余不能被SSE优化的数据
	return Sum;
}
void _mm_storesi128_4char(__m128i Src, unsigned char* Dest)
{
	__m128i T = _mm_packs_epi32(Src, Src);
	T = _mm_packus_epi16(T, T);
	*((int*)Dest) = _mm_cvtsi128_si32(T);
}

/// <summary>
/// 将整数限幅到字节数据类型。
/// </summary>
inline unsigned char IM_ClampToByte(int Value)            //    现代PC还是这样直接写快些
{
	if (Value < 0)
		return 0;
	else if (Value > 255)
		return 255;
	else
		return (unsigned char)Value;
	//return ((Value | ((signed int)(255 - Value) >> 31)) & ~((signed int)Value >> 31));
}

inline unsigned char IM_Min(int a, int b)
{
	if (a < b)
		return a;
	else if (a > b)
		return b;
}
