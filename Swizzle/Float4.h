#pragma once

#include <smmintrin.h>

class __declspec(align(16)) float4
{
public:
	float4()
	{
		f32[0] = 0.0f;
		f32[1] = 0.0f;
		f32[2] = 0.0f;
		f32[3] = 1.0f;
	}

	float4(float f)
	{
		vec = _mm_set1_ps(f);
	}

	float4(float x, float y, float z, float w)
	{
		vec = _mm_setr_ps(x, y, z, w);
	}

	float4 operator=(const float4& f)
	{
		vec = f.vec;
		return *this;
	}

	union
	{
		__m128 vec;
		float f32[4];
#include "Swizzle_x.h"
#include "Swizzle_y.h"
#include "Swizzle_z.h"
#include "Swizzle_w.h"
	};
};