/*=============================================================================
	KVector.h
=============================================================================*/

#ifndef __INC_FVECTOR
#define __INC_FVECTOR

/*-----------------------------------------------------------------------------
	CVec2
-----------------------------------------------------------------------------*/

#define EPSILON_E4		(1.e-4)
#define EPSILON_E8		(1.e-8)

#include <math.h>

class CVec2
{
public:

	float	x;
	float	y;
			
	// Constructors

	CVec2()
	{}
	CVec2(const float _x, const float _y)
		: x(_x), y(_y)
	{}

	// Binary operators	
	inline CVec2 operator+(const CVec2& v) const
	{
		return(CVec2(x + v.x, y + v.y));
	}
	inline CVec2 operator-(const CVec2& v) const
	{
		return(CVec2(x - v.x, y - v.y));
	}
	inline CVec2 operator*(const CVec2& v) const
	{
		return(CVec2(x * v.x, y * v.y));
	}
	inline CVec2 operator/(const CVec2& v) const
	{
		return(CVec2(x / v.x, y / v.y));
	}
	inline CVec2 operator*(const float s) const
	{
		return(CVec2(s * x, s * y));
	}
	friend inline CVec2 operator*(const float fScale, const CVec2& v)
	{
		return(CVec2(fScale * v.x, fScale * v.y));
	}
	inline CVec2 operator/(const float fScale) const
	{
		float fRcpScale = 1.0f / fScale;
		return(CVec2(x * fRcpScale, y * fRcpScale));
	}

	// Unary operators
	inline CVec2 operator-() const
	{
		return CVec2(-x, -y);
	}
	inline float operator[](int iIndex) const
	{
		return (&this->x)[iIndex];
	}
	inline float& operator[](int iIndex)
	{
		return (&this->x)[iIndex];
	}

	// Comparison operators
	inline bool operator==(const CVec2& v) const
	{
		return((x == v.x) && (y == v.y));
	}
	inline bool operator!=(const CVec2& v) const
	{
		return((x != v.x) || (y != v.y));
	}

	// Assignment operators
	inline CVec2 operator+=(const CVec2& v)
	{
		x += v.x, y += v.y;
		return *this;
	}
	inline CVec2 operator-=(const CVec2& v)
	{
		x -= v.x, y -= v.y;
		return *this;
	}
	inline CVec2 operator*=(const CVec2& v)
	{
		x *= v.x, y *= v.y;
		return *this;
	}
	inline CVec2 operator/=(const CVec2& v)
	{
		x /= v.x, y /= v.y;
		return *this;
	}
	inline CVec2 operator*=(const float fScale)
	{
		x *= fScale, y *= fScale;
			
		return *this;
	}
	inline CVec2 operator/=(const float fScale)
	{
		float fRcpScale = 1.0f / fScale;
		x *= fRcpScale; y *= fRcpScale;
		return *this;
	}

	// Functions
	inline void Set(const float _x, const float _y)
	{
		x = _x,	y = _y;
	}
	inline void Zero()
	{
		x = 0, y = 0;
	}
	inline bool IsZero()
	{
		return ((x == 0) && (y == 0));
	}
	inline bool IsNearlyZero()
	{
		return((x < EPSILON_E4) && (y < EPSILON_E4));
	}
	inline void Normalize()
	{
		float fRcpLength = 1.0f / (float)sqrt((x * x) + (y * y));
		x *= fRcpLength, y *= fRcpLength;
	}
	inline float Length()
	{
		return ((float)sqrt((x * x) + (y * y)));
	}
	inline float LengthSquared()
	{
		return((x * x) + (y * y));
	}
	inline float Dot(const CVec2& v) const
	{
		return((x * v.x) + (y * v.y));
	}
	inline CVec2 Perp() const
	{
		return CVec2(-y, x);
	}	
	inline float* Ptr()
	{
		return &x;
	}
	inline const float* Ptr() const
	{
		return &x;
	}
	
	// Friend functions
	friend inline float Dot(const CVec2& a, const CVec2& b)
	{
		return((a.x * b.x) + (a.y * b.y));
	}
};

/*-----------------------------------------------------------------------------
	CVec3
-----------------------------------------------------------------------------*/

class CVec3
{
public:

	float	x;
	float	y;
	float	z;
			
	// Constructors
	CVec3()
	{}
	CVec3(const float _x, const float _y, const float _z)
		: x(_x), y(_y), z(_z)
	{}

	// Binary operators	
	inline CVec3 operator+(const CVec3& v) const
	{
		return(CVec3(x + v.x, y + v.y, z + v.z));
	}
	inline CVec3 operator-(const CVec3& v) const
	{
		return(CVec3(x - v.x, y - v.y, z - v.z));
	}
	inline CVec3 operator*(const CVec3& v) const
	{
		return(CVec3(x * v.x, y * v.y, z * v.z));
	}
	inline CVec3 operator/(const CVec3& v) const
	{
		return(CVec3(x / v.x, y / v.y, z / v.z));
	}
	inline CVec3 operator*(const float s) const
	{
		return(CVec3(s * x, s * y, s * z));
	}
	friend inline CVec3 operator*(const float fScale, const CVec3& v)
	{
		return(CVec3(fScale * v.x, fScale * v.y, fScale * v.z));
	}
	inline CVec3 operator/(const float fScale) const
	{
		float fRcpScale = 1.0f / fScale;
		return(CVec3(x * fRcpScale, y * fRcpScale, z * fRcpScale));
	}

	// Unary operators
	inline CVec3 operator-() const
	{
		return CVec3(-x, -y, -z);
	}
	inline float operator[](int iIndex) const
	{
		return (&this->x)[iIndex];
	}
	inline float& operator[](int iIndex)
	{
		return (&this->x)[iIndex];
	}

	// Comparison operators
	inline bool operator==(const CVec3& v) const
	{
		return((x == v.x) && (y == v.y) && (z == v.z));
	}
	inline bool operator!=(const CVec3& v) const
	{
		return((x != v.x) || (y != v.y) || (z != v.z));
	}

	// Assignment operators
	inline CVec3 operator+=(const CVec3& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	inline CVec3 operator-=(const CVec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	inline CVec3 operator*=(const CVec3& v)
	{
		x *= v.x; y *= v.y; z *= v.z;
		return *this;
	}
	inline CVec3 operator/=(const CVec3& v)
	{
		x /= v.x; y /= v.y; z /= v.z;
		return *this;
	}
	inline CVec3 operator*=(const float fScale)
	{
		x *= fScale; y *= fScale; z *= fScale;
			
		return *this;
	}
	inline CVec3 operator/=(const float fScale)
	{
		float fRcpScale = 1.0f / fScale;
		x *= fRcpScale; y *= fRcpScale; z *= fRcpScale;
		return *this;
	}

	// Functions
	inline void Set(const float _x, const float _y, const float _z)
	{
		x = _x;	y = _y;	z = _z;
	}
	inline void Zero()
	{
		x = 0; y = 0; z = 0;
	}
	inline bool IsZero()
	{
		return ((x == 0) && (y == 0) && (z == 0));
	}
	inline bool IsNearlyZero()
	{
		return((x < EPSILON_E4) && (y < EPSILON_E4) && (z < EPSILON_E4));
	}
	inline CVec3 Normalize()
	{
		float fRcpLength = 1.0f / (float)sqrt((x * x) + (y * y) + (z * z));
		x *= fRcpLength, y *= fRcpLength, z *= fRcpLength;

		return *this;
	}
	inline float Length()
	{
		return ((float)sqrt((x * x) + (y * y) + (z * z)));
	}
	inline float LengthSquared()
	{
		return((x * x) + (y * y) + (z * z));
	}
	inline float Dot(const CVec3& v) const
	{
		return((x * v.x) + (y * v.y) + (z * v.z));
	}
	inline CVec3 Cross(const CVec3& v) const
	{
		return(CVec3((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x)));
	}
	inline float* Ptr()
	{
		return &x;
	}
	inline const float* Ptr() const
	{
		return &x;
	}
};

/*-----------------------------------------------------------------------------
	CVec4
-----------------------------------------------------------------------------*/

class CVec4
{
public:
	
	float	x;
	float	y;
	float	z;
	float	w;
		
	// Constructors
	CVec4()
	{}
	CVec4(const float _x, const float _y, const float _z, const float _w)
		: x(_x), y(_y), z(_z), w(_w)
	{}

	// Binary operators	
	inline CVec4 operator+(const CVec4& v) const
	{
		return(CVec4(x + v.x, y + v.y, z + v.z, w + v.w));
	}
	inline CVec4 operator-(const CVec4& v) const
	{
		return(CVec4(x - v.x, y - v.y, z - v.z, w - v.w));
	}
	inline CVec4 operator*(const CVec4& v) const
	{
		return(CVec4(x * v.x, y * v.y, z * v.z, w * v.w));
	}
	inline CVec4 operator/(const CVec4& v) const
	{
		return(CVec4(x / v.x, y / v.y, z / v.z, w / v.w));
	}
	inline CVec4 operator*(const float s) const
	{
		return(CVec4(s * x, s * y, s * z, s * w));
	}
	friend inline CVec4 operator*(const float fScale, const CVec4& v)
	{
		return(CVec4(fScale * v.x, fScale * v.y, fScale * v.z, fScale * v.w));
	}
	inline CVec4 operator/(const float fScale) const
	{
		float fRcpScale = 1.0f / fScale;
		return(CVec4(x * fRcpScale, y * fRcpScale, z * fRcpScale, w * fRcpScale));
	}

	// Unary operators
	inline CVec4 operator-() const
	{
		return CVec4(-x, -y, -z, -w);
	}
	inline float operator[](int iIndex) const
	{
		return (&this->x)[iIndex];
	}
	inline float& operator[](int iIndex)
	{
		return (&this->x)[iIndex];
	}

	// Comparison operators
	inline bool operator==(const CVec4& v) const
	{
		return((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
	}
	inline bool operator!=(const CVec4& v) const
	{
		return((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w));
	}

	// Assignment operators
	inline CVec4 operator+=(const CVec4& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}
	inline CVec4 operator-=(const CVec4& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}
	inline CVec4 operator*=(const CVec4& v)
	{
		x *= v.x; y *= v.y; z *= v.z; w *= v.w;
		return *this;
	}
	inline CVec4 operator/=(const CVec4& v)
	{
		x /= v.x; y /= v.y; z /= v.z; w /= v.w;
		return *this;
	}
	inline CVec4 operator*=(const float fScale)
	{
		x *= fScale; y *= fScale; z *= fScale; w *= fScale;
			
		return *this;
	}
	inline CVec4 operator/=(const float fScale)
	{
		float fRcpScale = 1.0f / fScale;
		x *= fRcpScale; y *= fRcpScale; z *= fRcpScale;	w *= fRcpScale;
		return *this;
	}

	// Functions
	inline void Set(const float _x, const float _y, const float _z, const float _w)
	{
		x = _x;	y = _y;	z = _z;	w = _w;
	}
	inline void Zero()
	{
		x = 0; y = 0; z = 0; w = 0;
	}
	inline bool IsZero()
	{
		return ((x == 0) && (y == 0) && (z == 0) && (w == 0));
	}
	inline bool IsNearlyZero()
	{
		return((x < EPSILON_E4) && (y < EPSILON_E4) && (z < EPSILON_E4) && (w < EPSILON_E4));
	}
	inline CVec4 Normalize()
	{
		float fRcpLength = 1.0f / (float)sqrt((x * x) + (y * y) + (z * z) + (w * w));
		x *= fRcpLength, y *= fRcpLength, z *= fRcpLength, w *= fRcpLength;

		return *this;
	}
	inline float Length()
	{
		return ((float)sqrt((x * x) + (y * y) + (z * z) + (w * w)));
	}
	inline float LengthSquared()
	{
		return((x * x) + (y * y) + (z * z) + (w * w));
	}
	inline float Dot(const CVec4& v) const
	{
		return((x * v.x) + (y * v.y) + (z * v.z) + (w * v.w));
	}
	inline float* Ptr()
	{
		return &x;
	}
	inline const float* Ptr() const
	{
		return &x;
	}
};

/*-----------------------------------------------------------------------------
	CVec6
-----------------------------------------------------------------------------*/

class CVec6
{
public:
	
	float m[6];
		
	// Constructors
	CVec6()
	{}
	CVec6(const float v0, const float v1, const float v2, const float v3, const float v4, const float v5)
	{
		m[0] = v0;
		m[1] = v1;
		m[2] = v2;
		m[3] = v3;
		m[4] = v4;
		m[5] = v5;
	}

	// Binary operators	
	inline CVec6 operator+(const CVec6& v) const
	{
		return(CVec6(m[0] + v.m[0], m[1] + v.m[1], m[2] + v.m[2], m[3] + v.m[3], m[4] + v.m[4], m[5] + v.m[5]));
	}
	inline CVec6 operator-(const CVec6& v) const
	{
		return(CVec6(m[0] - v.m[0], m[1] - v.m[1], m[2] - v.m[2], m[3] - v.m[3], m[4] - v.m[4], m[5] - v.m[5]));
	}
	inline CVec6 operator*(const CVec6& v) const
	{
		return(CVec6(m[0] * v.m[0], m[1] * v.m[1], m[2] * v.m[2], m[3] * v.m[3], m[4] * v.m[4], m[5] * v.m[5]));
	}
	inline CVec6 operator/(const CVec6& v) const
	{
		return(CVec6(m[0] * v.m[0], m[1] * v.m[1], m[2] * v.m[2], m[3] * v.m[3], m[4] * v.m[4], m[5] * v.m[5]));
	}
	inline CVec6 operator*(const float s) const
	{
		return(CVec6(s * m[0], s * m[1], s * m[2], s * m[3], s * m[4], s * m[5]));
	}
	friend inline CVec6 operator*(const float s, const CVec6& v)
	{
		return(CVec6(s * v.m[0], s * v.m[1], s * v.m[2], s * v.m[3], s * v.m[4], s * v.m[5]));
	}
	inline CVec6 operator/(const float fScale) const
	{
		float f = 1.0f / fScale;
		return(CVec6(f * m[0], f * m[1], f * m[2], f * m[3], f * m[4], f * m[5]));
	}

	// Unary operators
	inline CVec6 operator-() const
	{
		return CVec6(-m[0], -m[1], -m[2], -m[3], -m[4], -m[5]);
	}
	inline float operator[]( int iIndex ) const
	{
		return (this->m)[iIndex];
	}
	inline float& operator[]( int iIndex )
	{
		return (this->m)[iIndex];
	}

	// Comparison operators
	inline bool operator==(const CVec6& v) const
	{
		return((m[0] == v.m[0]) && (m[1] == v.m[1]) && (m[2] == v.m[2]) && (m[3] == v.m[3]) && (m[4] == v.m[4]) && (m[5] == v.m[5]));
	}
	inline bool operator!=(const CVec6& v) const
	{
		return((m[0] != v.m[0]) && (m[1] != v.m[1]) && (m[2] != v.m[2]) && (m[3] != v.m[3]) && (m[4] != v.m[4]) && (m[5] != v.m[5]));
	}

	// Assignment operators
	inline CVec6 operator+=(const CVec6& v)
	{
		m[0] += v.m[0]; m[1] += v.m[1]; m[2] += v.m[2]; m[3] += v.m[3]; m[4] += v.m[4]; m[5] += v.m[5];
		return *this;
	}
	inline CVec6 operator-=(const CVec6& v)
	{
		m[0] -= v.m[0]; m[1] -= v.m[1]; m[2] -= v.m[2]; m[3] -= v.m[3]; m[4] -= v.m[4]; m[5] -= v.m[5];
		return *this;
	}
	inline CVec6 operator*=(const CVec6& v)
	{
		m[0] *= v.m[0]; m[1] *= v.m[1]; m[2] *= v.m[2]; m[3] *= v.m[3]; m[4] *= v.m[4]; m[5] *= v.m[5];
		return *this;
	}
	inline CVec6 operator/=(const CVec6& v)
	{
		m[0] /= v.m[0]; m[1] /= v.m[1]; m[2] /= v.m[2]; m[3] /= v.m[3]; m[4] /= v.m[4]; m[5] /= v.m[5];
		return *this;
	}
	inline CVec6 operator*=(const float f)
	{
		m[0] *= f; m[1] *= f; m[2] *= f; m[3] *= f; m[4] *= f; m[5] *= f;
		return *this;
	}
	inline CVec6 operator/=(const float fScale)
	{
		float f = 1.0f / fScale;
		m[0] *= f; m[1] *= f; m[2] *= f; m[3] *= f; m[4] *= f; m[5] *= f;
		return *this;
	}

	// Functions
	inline void Set(const float v0, const float v1, const float v2, const float v3, const float v4, const float v5)
	{
		m[0] = v0; m[1] = v1; m[2] = v2; m[3] = v3; m[4] = v4; m[5] = v5;
	}
	inline void Zero()
	{
		m[0] = 0; m[1] = 0; m[2] = 0; m[3] = 0; m[4] = 0; m[5] = 0;
	}
	inline bool IsZero()
	{
		return ((m[0] == 0) && (m[1] == 0) && (m[2] == 0) && (m[3] == 0) && (m[4] == 0) && (m[5] == 0));
	}
	inline bool IsNearlyZero()
	{
		return((m[0] < EPSILON_E4) && (m[1] < EPSILON_E4) && (m[2] < EPSILON_E4) && (m[3] < EPSILON_E4) && (m[4] < EPSILON_E4) && (m[5] < EPSILON_E4));
	}
	inline void Normalize()
	{
		float f = 1.0f / (float)sqrt((m[0] * m[0]) + (m[1] * m[1]) + (m[2] * m[2]) + (m[3] * m[3]) + (m[4] * m[4]) + (m[5] * m[5]));
		m[0] *= f, m[1] *= f, m[2] *= f, m[3] *= f; m[4] *= f; m[5] *= f;
	}
	inline float Length()
	{
		return((float)sqrt((m[0] * m[0]) + (m[1] * m[1]) + (m[2] * m[2]) + (m[3] * m[3]) + (m[4] * m[4]) + (m[5] * m[5])));
	}
	inline float LengthSquared()
	{
		return((m[0] * m[0]) + (m[1] * m[1]) + (m[2] * m[2]) + (m[3] * m[3]) + (m[4] * m[4]) + (m[5] * m[5]));
	}
	inline float Dot(const CVec6& v) const
	{
		return(m[0] * v.m[0] + m[1] * v.m[1] + m[2] * v.m[2] + m[3] * v.m[3] + m[4] * v.m[4] + m[5] * v.m[5]);
	}
	inline CVec3 SubVector3(int nIndex) const
	{
		if(nIndex == 0)
		{
			return CVec3(m[0], m[1], m[2]);
		}
		else
		{
			return CVec3(m[3], m[4], m[5]);
		}
	}
	inline float* Ptr()
	{
		return m;
	}
	inline const float* Ptr() const
	{
		return m;
	}
};

#endif

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/


