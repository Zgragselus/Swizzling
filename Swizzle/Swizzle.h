#pragma once

#define SHUFFLE_MASK(X, Y, Z, W)		(((W) << 6) | ((Z) << 4) | ((Y) << 2) | (X))

#define COMPONENT_X(X)				(1 << X)
#define COMPONENT_XY(X, Y)			((1 << X) | (1 << Y))
#define COMPONENT_XYZ(X, Y, Z)		((1 << X) | (1 << Y) | (1 << Z))
#define COMPONENT_XYZW(X, Y, Z, W)	((1 << X) | (1 << Y) | (1 << Z) | (1 << W))

#define IDENTITY_MASK ((3 << 6) | (2 << 4) | (1 << 2) | 0)

template<int X>
struct swizzle1
{
private:
	union { __m128 vec; float f32[4]; };

public:
	template<int A> friend struct swizzle1;

	__forceinline operator float() const { return f32[X]; }

	template<int E, int A>
	static __forceinline __m128 swizzle(__m128 v)
	{
		const int finalMask = (((IDENTITY_MASK >> 2 * E) & 3) << 2 * A) | (IDENTITY_MASK & ~((3 << 2 * A)));
		return _mm_shuffle_ps(v, v, finalMask);
	}

	template<int E, int A>
	__forceinline __m128 swizzle() const
	{
		return swizzle<E, A>(vec);
	}

	__forceinline swizzle1& operator = (float f)
	{
		vec = _mm_blend_ps(vec, _mm_set1_ps(f), COMPONENT_X(X));
		return *this;
	}

	template<int A>
	__forceinline swizzle1& operator = (const swizzle1<A>& s)
	{
		__m128 t = _mm_shuffle_ps(s.vec, s.vec, SHUFFLE_MASK(A, A, A, A));
		vec = _mm_blend_ps(vec, t, COMPONENT_X(X));
		return *this;
	}

	__forceinline swizzle1& operator = (const swizzle1<X>& s)
	{
		__m128 t = _mm_shuffle_ps(s.vec, s.vec, SHUFFLE_MASK(X, X, X, X));
		vec = _mm_blend_ps(vec, t, COMPONENT_X(X));
		return *this;
	}
};

template<int X, int Y>
struct swizzle2
{
private:
	union { __m128 vec; float f32[4]; };

public:
	void staticAsserts()
	{
		static_assert(X != Y, "L-value is const object - no component can be equal for assignment");
	}

	template<int E, int F, int A, int B>
	static __forceinline __m128 swizzle(__m128 v)
	{
		const int finalMask =
			(((IDENTITY_MASK >> 2 * E) & 3) << 2 * A) |
			(((IDENTITY_MASK >> 2 * F) & 3) << 2 * B) |
			(IDENTITY_MASK & ~((3 << 2 * A) | (3 << 2 * B)));
		return _mm_shuffle_ps(v, v, finalMask);
	}

	template<int E, int F, int A, int B>
	__forceinline __m128 swizzle() const
	{
		return swizzle<E, F, A, B>(vec);
	}

	template<int A, int B>
	__forceinline swizzle2& operator = (const swizzle2<A, B>& s)
	{
		staticAsserts();
		vec = blend(vec, s.template swizzle<A, B, X, Y>());
		return *this;
	}

	__forceinline swizzle2& operator = (const swizzle2<X, Y>& s)
	{
		staticAsserts();
		vec = blend(vec, s.template swizzle<X, Y, X, Y>());
		return *this;
	}

private:

	static __forceinline __m128 blend(__m128 x, __m128 y)
	{
		return _mm_blend_ps(x, y, COMPONENT_XY(X, Y));
	}
};

template<int X, int Y, int Z>
struct swizzle3
{
private:
	union { __m128 vec; float f32[4]; };

public:
	void staticAsserts()
	{
		static_assert(X != Y && X != Z && Y != Z, "L-value is const object - no component can be equal for assignment");
	}

	template<int E, int F, int G, int A, int B, int C>
	static __forceinline __m128 swizzle(__m128 v)
	{
		const int finalMask =
			(((IDENTITY_MASK >> 2 * E) & 3) << 2 * A) |
			(((IDENTITY_MASK >> 2 * F) & 3) << 2 * B) |
			(((IDENTITY_MASK >> 2 * G) & 3) << 2 * C) |
			(IDENTITY_MASK & ~((3 << 2 * A) | (3 << 2 * B) | (3 << 2 * C)));
		return _mm_shuffle_ps(v, v, finalMask);
	}

	template<int E, int F, int G, int A, int B, int C>
	__forceinline __m128 swizzle() const
	{
		return swizzle<E, F, G, A, B, C>(vec);
	}

	template<int A, int B, int C>
	__forceinline swizzle3& operator = (const swizzle3<A, B, C>& s)
	{
		staticAsserts();
		vec = blend(vec, s.template swizzle<A, B, C, X, Y, Z>());
		return *this;
	}

	__forceinline swizzle3& operator = (const swizzle3<X, Y, Z>& s)
	{
		staticAsserts();
		vec = blend(vec, s.template swizzle<X, Y, Z, X, Y, Z>());
		return *this;
	}

private:

	static __forceinline __m128 blend(__m128 x, __m128 y)
	{
		return _mm_blend_ps(x, y, COMPONENT_XYZ(X, Y, Z)); 
	}
};

template<int X, int Y, int Z, int W>
struct swizzle4
{
private:
	union { __m128 vec; float f32[4]; };

public:
	void staticAsserts()
	{
		static_assert(X != Y && X != Z && X != W && Y != Z && Y != W && Z != W, "L-value is const object - no component can be equal for assignment");
	}

	template<int E, int F, int G, int H, int A, int B, int C, int D>
	static __forceinline __m128 swizzle(__m128 v)
	{
		const int finalMask =
			(((IDENTITY_MASK >> 2 * E) & 3) << (2 * A)) |
			(((IDENTITY_MASK >> 2 * F) & 3) << (2 * B)) |
			(((IDENTITY_MASK >> 2 * G) & 3) << (2 * C)) |
			(((IDENTITY_MASK >> 2 * H) & 3) << (2 * D));

		return _mm_shuffle_ps(v, v, finalMask);
	}

	template<int E, int F, int G, int H, int A, int B, int C, int D>
	__forceinline __m128 swizzle() const
	{
		return swizzle<E, F, G, H, A, B, C, D>(vec);
	}

	template<int A, int B, int C, int D>
	__forceinline swizzle4& operator = (const swizzle4<A, B, C, D>& s)
	{
		staticAsserts();
		vec = s.template swizzle<A, B, C, D, X, Y, Z, W>();
		return *this;
	}

	__forceinline swizzle4& operator = (const swizzle4<X, Y, Z, W>& s)
	{
		staticAsserts();
		vec = s.template swizzle<X, Y, Z, W, X, Y, Z, W>();
		return *this;
	}
};