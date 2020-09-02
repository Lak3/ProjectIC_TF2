#pragma once
#include <math.h>
#include <cmath>		//std::isfinite std::remainder
#include <cfloat>		//FLT_EPSILON
#include <algorithm>	//std::min std::max

typedef float vec_t;

// Used to represent sides of things like planes.
#define	SIDE_FRONT	0
#define	SIDE_BACK	1
#define	SIDE_ON		2
#define VP_EPSILON	0.01f

#define M_RADPI		57.295779513082
#define PI			3.14159265358979323846
#define DEG2RAD(x)	((float)(x) * (float)((float)(PI) / 180.0f))
#define RAD2DEG(x)	((float)(x) * (float)(180.0f / (float)(PI)))

#pragma warning (disable : 26495)
#pragma warning (disable : 26451)
#pragma warning (disable : 4244)

class Vec2
{
public:
	Vec2(void)
	{
		x = y = 0.0f;
	}

	Vec2(float X, float Y)
	{
		x = X; y = Y;
	}

	Vec2(float* v)
	{
		x = v[0]; y = v[1];
	}

	Vec2(const float* v)
	{
		x = v[0]; y = v[1];
	}

	Vec2(const Vec2& v)
	{
		x = v.x; y = v.y;
	}

	Vec2& operator=(const Vec2& v)
	{
		x = v.x; y = v.y; return *this;
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	Vec2& operator+=(const Vec2& v)
	{
		x += v.x; y += v.y; return *this;
	}

	Vec2& operator-=(const Vec2& v)
	{
		x -= v.x; y -= v.y; return *this;
	}

	Vec2& operator*=(const Vec2& v)
	{
		x *= v.x; y *= v.y; return *this;
	}

	Vec2& operator/=(const Vec2& v)
	{
		x /= v.x; y /= v.y; return *this;
	}

	Vec2& operator+=(float v)
	{
		x += v; y += v; return *this;
	}

	Vec2& operator-=(float v)
	{
		x -= v; y -= v; return *this;
	}

	Vec2& operator*=(float v)
	{
		x *= v; y *= v; return *this;
	}

	Vec2& operator/=(float v)
	{
		x /= v; y /= v; return *this;
	}

	Vec2 operator+(const Vec2& v) const
	{
		return Vec2(x + v.x, y + v.y);
	}

	Vec2 operator-(const Vec2& v) const
	{
		return Vec2(x - v.x, y - v.y);
	}

	Vec2 operator*(const Vec2& v) const
	{
		return Vec2(x * v.x, y * v.y);
	}

	Vec2 operator/(const Vec2& v) const
	{
		return Vec2(x / v.x, y / v.y);
	}

	Vec2 operator+(float v) const
	{
		return Vec2(x + v, y + v);
	}

	Vec2 operator-(float v) const
	{
		return Vec2(x - v, y - v);
	}

	Vec2 operator*(float v) const
	{
		return Vec2(x * v, y * v);
	}

	Vec2 operator/(float v) const
	{
		return Vec2(x / v, y / v);
	}

	void Set(float X = 0.0f, float Y = 0.0f)
	{
		x = X; y = Y;
	}

	float Lenght(void) const
	{
		return sqrtf(x * x + y * y);
	}

	float LenghtSqr(void) const
	{
		return (x * x + y * y);
	}

	float DistTo(const Vec2& v) const
	{
		return (*this - v).Lenght();
	}

	float DistToSqr(const Vec2& v) const
	{
		return (*this - v).LenghtSqr();
	}

	float Dot(const Vec2& v) const
	{
		return (x * v.x + y * v.y);
	}

	bool IsZero(void) const
	{
		return (x > -0.01f && x < 0.01f &&
				y > -0.01f && y < 0.01f);
	}

public:
	float x, y;
};

class Vec3
{
public:
	Vec3(void)
	{
		x = y = z = 0.0f;
	}

	void Zero()
	{
		x = y = z = 0.f;
	}

	Vec3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	Vec3(float* v)
	{
		x = v[0]; y = v[1]; z = v[2];
	}

	Vec3(const float* v)
	{
		x = v[0]; y = v[1]; z = v[2];
	}

	Vec3(const Vec3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}

	Vec3(const Vec2& v)
	{
		x = v.x; y = v.y; z = 0.0f;
	}

	Vec3& operator=(const Vec3& v)
	{
		x = v.x; y = v.y; z = v.z; return *this;
	}

	Vec3& operator=(const Vec2& v)
	{
		x = v.x; y = v.y; z = 0.0f; return *this;
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	Vec3& operator+=(const Vec3& v)
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}

	Vec3& operator-=(const Vec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	Vec3& operator*=(const Vec3& v)
	{
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}

	Vec3& operator/=(const Vec3& v)
	{
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}

	Vec3& operator+=(float v)
	{
		x += v; y += v; z += v; return *this;
	}

	Vec3& operator-=(float v)
	{
		x -= v; y -= v; z -= v; return *this;
	}

	Vec3& operator*=(float v)
	{
		x *= v; y *= v; z *= v; return *this;
	}

	Vec3& operator/=(float v)
	{
		x /= v; y /= v; z /= v; return *this;
	}

	Vec3 operator+(const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator-(const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator*(const Vec3& v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3 operator/(const Vec3& v) const
	{
		return Vec3(x / v.x, y / v.y, z / v.z);
	}

	Vec3 operator+(float v) const
	{
		return Vec3(x + v, y + v, z + v);
	}

	Vec3 operator-(float v) const
	{
		return Vec3(x - v, y - v, z - v);
	}

	Vec3 operator*(float v) const
	{
		return Vec3(x * v, y * v, z * v);
	}

	Vec3 operator/(float v) const
	{
		return Vec3(x / v, y / v, z / v);
	}

	void Set(float X = 0.0f, float Y = 0.0f, float Z = 0.0f)
	{
		x = X; y = Y; z = Z;
	}

	float Lenght(void) const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float LenghtSqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	float Normalize()
	{
		float fl_lenght = Lenght();
		float fl_lenght_normal = 1.f / (FLT_EPSILON + fl_lenght);

		x = x * fl_lenght_normal;
		y = y * fl_lenght_normal;
		z = z * fl_lenght_normal;

		return fl_lenght;
	}

	float NormalizeInPlace()
	{
		return Normalize();
	}

	float Lenght2D(void) const
	{
		return sqrtf(x * x + y * y);
	}

	float Lenght2DSqr(void) const
	{
		return (x * x + y * y);
	}

	float DistTo(const Vec3& v) const
	{
		return (*this - v).Lenght();
	}

	float DistToSqr(const Vec3& v) const
	{
		return (*this - v).LenghtSqr();
	}

	float Dot(const Vec3& v) const
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	Vec3 Cross(const Vec3& v) const
	{
		return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	bool IsZero(void) const
	{
		return (x > -0.01f && x < 0.01f &&
				y > -0.01f && y < 0.01f &&
				z > -0.01f && z < 0.01f);
	}

	Vec3 Scale(float fl) {
		return Vec3(x * fl, y * fl, z * fl);
	}

	void Init(float ix, float iy, float iz)
	{
		x = ix; y = iy; z = iz;
	}

public:
	float x, y, z;
};

class __declspec(align(16))VectorAligned : public Vec3
{
public:
	inline VectorAligned(void) { };

	inline VectorAligned(float x, float y, float z) {
		Set(x, y, z);
	}

	explicit VectorAligned(const Vec3& othr) {
		Set(othr.x, othr.y, othr.z);
	}

	VectorAligned& operator=(const Vec3& othr) {
		Set(othr.x, othr.y, othr.z);
		return *this;
	}

	float w;
};

typedef float matrix3x4[3][4];

namespace Math
{
	inline double __declspec (naked) __fastcall FastSqrt(double n)
	{
		_asm fld qword ptr[esp + 4]
			_asm fsqrt
		_asm ret 8
	}

	inline void SinCos(float radians, float* sine, float* cosine)
	{
		_asm
		{
			fld		DWORD PTR[radians]
			fsincos

			mov edx, DWORD PTR[cosine]
			mov eax, DWORD PTR[sine]

			fstp DWORD PTR[edx]
			fstp DWORD PTR[eax]
		}
	}

	inline float NormalizeAngle(float ang)
	{
		if (!std::isfinite(ang))
			return 0.0f;

		return std::remainder(ang, 360.0f);
	}

	inline void ClampAngles(Vec3& v)
	{
		v.x = std::max(-89.0f, std::min(89.0f, NormalizeAngle(v.x)));
		v.y = NormalizeAngle(v.y);
		v.z = 0.0f;
	}

	inline void VectorTransform(const Vec3& input, const matrix3x4& matrix, Vec3& output)
	{
		for (auto i = 0; i < 3; i++)
			output[i] = input.Dot((Vec3&)matrix[i]) + matrix[i][3];
	}

	inline void AngleVectors(const Vec3& angles, Vec3* forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles.x), &sp, &cp);
		SinCos(DEG2RAD(angles.y), &sy, &cy);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}
	}

	inline void AngleVectors(const Vec3 &angles, Vec3 *forward, Vec3 *right, Vec3 *up)
	{
		float sr, sp, sy, cr, cp, cy;
		SinCos(DEG2RAD(angles.x), &sp, &cp);
		SinCos(DEG2RAD(angles.y), &sy, &cy);
		SinCos(DEG2RAD(angles.z), &sr, &cr);

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right) {
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up) {
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	inline void AngleNormalize(Vec3& vector)
	{
		for (auto i = 0; i < 3; i++)
		{
			if (vector[i] < -180.0f) vector[i] += 360.0f;
			if (vector[i] > 180.0f) vector[i] -= 360.0f;
		}
	}

	inline float VectorNormalize(Vec3& vector)
	{
		float lenght = vector.Lenght();

		if (!lenght)
			vector.Set();
		else
			vector /= lenght;

		return lenght;
	}

	inline Vec3 CalcAngle(const Vec3& source, const Vec3& destination)
	{
		Vec3 angles = Vec3(0.0f, 0.0f, 0.0f);
		Vec3 delta = (source - destination);
		float fHyp = FastSqrt((delta.x * delta.x) + (delta.y * delta.y));

		angles.x = (atanf(delta.z / fHyp) * M_RADPI);
		angles.y = (atanf(delta.y / delta.x) * M_RADPI);
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}

	inline float CalcFov(const Vec3& src, const Vec3& dst)
	{
		Vec3 v_src = Vec3();
		AngleVectors(src, &v_src);

		Vec3 v_dst = Vec3();
		AngleVectors(dst, &v_dst);

		float result = RAD2DEG(acos(v_dst.Dot(v_src) / v_dst.LenghtSqr()));

		if (!isfinite(result) || isinf(result) || isnan(result))
			result = 0.0f;

		return result;
	}

	inline void CreateVector(const Vec3& angle, Vec3& vector)
	{
		float pitch, yaw, tmp;

		pitch = float(angle[0] * PI / 180);
		yaw = float(angle[1] * PI / 180);
		tmp = float(cos(pitch));

		vector[0] = float(-tmp * -cos(yaw));
		vector[1] = float(sin(yaw) * tmp);
		vector[2] = float(-sin(pitch));
	}

	inline void VectorAngles(const Vec3& forward, Vec3& angles)
	{
		float tmp, yaw, pitch;

		if (forward.y == 0 && forward.x == 0)
		{
			yaw = 0;

			if (forward.z > 0)
				pitch = 270;
			else
				pitch = 90;
		}

		else
		{
			yaw = RAD2DEG(atan2f(forward.y, forward.x));

			if (yaw < 0)
				yaw += 360;

			tmp = forward.Lenght2D();
			pitch = RAD2DEG(atan2f(-forward.z, tmp));

			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	inline float GetFov(const Vec3& angle, const Vec3& source, const Vec3& destination)
	{
		Vec3 ang, aim;
		float mag, u_dot_v;
		ang = CalcAngle(source, destination);

		CreateVector(angle, aim);
		CreateVector(ang, ang);

		mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
		u_dot_v = aim.Dot(ang);

		return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
	}

	inline Vec3 NormalizedAngle(const Vec3& angles)
	{
		static float sp, sy, cp, cy;

		Math::SinCos(DEG2RAD(angles.x), &sp, &cp);
		Math::SinCos(DEG2RAD(angles.y), &sy, &cy);

		Vec3 forward = { cp * cy, cp * sy, -sp };
		forward *= (1.f / (FLT_EPSILON + forward.Lenght()));
		return forward;
	}

	inline bool RayToAABB(const Vec3& origin, const Vec3& direction, const Vec3& vmin, const Vec3& vmax)
	{
		static float t[8];

		t[0] = (vmin.x - origin.x) / direction.x;
		t[1] = (vmax.x - origin.x) / direction.x;
		t[2] = (vmin.y - origin.y) / direction.y;
		t[3] = (vmax.y - origin.y) / direction.y;
		t[4] = (vmin.z - origin.z) / direction.z;
		t[5] = (vmax.z - origin.z) / direction.z;
		t[6] = fmax(fmax(fmin(t[0], t[1]), fmin(t[2], t[3])), fmin(t[4], t[5]));
		t[7] = fmin(fmin(fmax(t[0], t[1]), fmax(t[2], t[3])), fmax(t[4], t[5]));
		return !(t[7] < 0 || t[6] > t[7]);
	}

#define floatCompare(x, y) \
	(fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

	//adapted from https://github.com/gszauer/GamePhysicsCookbook/blob/15810bbf902c1cc19064c176a7e0626eda3b83bd/Code/Geometry3D.cpp#L584
	inline bool RayToOBB(const  Vec3& origin, const  Vec3& direction, const Vec3& position, const Vec3& min, const Vec3& max, const matrix3x4 orientation) {

		Vec3 p = position - origin;

		Vec3 X(orientation[0][0], orientation[0][1], orientation[0][2]);
		Vec3 Y(orientation[1][0], orientation[1][1], orientation[1][2]);
		Vec3 Z(orientation[2][0], orientation[2][1], orientation[2][2]);

		Vec3 f(
			X.Dot(direction),
			Y.Dot(direction),
			Z.Dot(direction)
		);

		Vec3 e(
			X.Dot(p),
			Y.Dot(p),
			Z.Dot(p)
		);

		float t[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; ++i) {
			if (floatCompare(f[i], 0)) {
				if (-e[i] + min[i] > 0 || -e[i] + max[i] < 0) {
					return false;
				}
				f[i] = 0.00001f; // Avoid div by 0!
			}

			t[i * 2 + 0] = (e[i] + max[i]) / f[i]; // tmin[x, y, z]
			t[i * 2 + 1] = (e[i] + min[i]) / f[i]; // tmax[x, y, z]
		}

		float tmin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));
		float tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));

		// if tmax < 0, ray is intersecting AABB
		// but entire AABB is behing it's origin
		if (tmax < 0) {
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax) {
			return false;
		}

		return true;
	}

	inline float RandFloat(float min, float max)
	{
		return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
	}

	inline float EaseInBack(float x)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		return c3 * x * x * x - c1 * x * x;
	}

	inline float EaseInOutSine(float x)
	{
		return -(cos(PI * x) - 1.0f) / 2.0f;
	}

	inline float MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
}

//I was supposed to implement this in properly
//But shit happens
class VPlane
{
public:
	VPlane();
	VPlane(const Vec3& vNormal, vec_t dist);

	void		Init(const Vec3& vNormal, vec_t dist);

	// Return the distance from the point to the plane.
	vec_t		DistTo(const Vec3& vVec) const;

	// Copy.
	VPlane& operator=(const VPlane& thePlane);

	// Returns SIDE_ON, SIDE_FRONT, or SIDE_BACK.
	// The epsilon for SIDE_ON can be passed in.
	int	GetPointSide(const Vec3& vPoint, vec_t sideEpsilon = VP_EPSILON) const;

	// Returns SIDE_FRONT or SIDE_BACK.
	int	GetPointSideExact(const Vec3& vPoint) const;

	// Classify the box with respect to the plane.
	// Returns SIDE_ON, SIDE_FRONT, or SIDE_BACK
	int	BoxOnPlaneSide(const Vec3& vMin, const Vec3& vMax) const;

	// Flip the plane.
	VPlane		Flip();

	// Get a point on the plane (normal*dist).
	Vec3		GetPointOnPlane() const;

	// Snap the specified point to the plane (along the plane's normal).
	Vec3		SnapPointToPlane(const Vec3& vPoint) const;

public:
	Vec3		m_Normal;
	vec_t		m_Dist;

#ifdef VECTOR_NO_SLOW_OPERATIONS
private:
	// No copy constructors allowed if we're in optimal mode
	VPlane(const VPlane& vOther);
#endif
};


//-----------------------------------------------------------------------------
// Inlines.
//-----------------------------------------------------------------------------
inline VPlane::VPlane()
{
}

inline VPlane::VPlane(const Vec3& vNormal, vec_t dist)
{
	m_Normal = vNormal;
	m_Dist = dist;
}

inline void	VPlane::Init(const Vec3& vNormal, vec_t dist)
{
	m_Normal = vNormal;
	m_Dist = dist;
}

inline vec_t VPlane::DistTo(const Vec3& vVec) const
{
	return vVec.Dot(m_Normal) - m_Dist;
}

inline VPlane& VPlane::operator=(const VPlane& thePlane)
{
	m_Normal = thePlane.m_Normal;
	m_Dist = thePlane.m_Dist;
	return *this;
}

inline VPlane VPlane::Flip()
{
	//This is very much fucking wrong
	return VPlane(m_Normal, -m_Dist);
}

inline Vec3 VPlane::GetPointOnPlane() const
{
	return m_Normal * m_Dist;
}

inline Vec3 VPlane::SnapPointToPlane(const Vec3& vPoint) const
{
	return vPoint - m_Normal * DistTo(vPoint);
}

inline int VPlane::GetPointSide(const Vec3& vPoint, vec_t sideEpsilon) const
{
	vec_t fDist;

	fDist = DistTo(vPoint);
	if (fDist >= sideEpsilon)
		return SIDE_FRONT;
	else if (fDist <= -sideEpsilon)
		return SIDE_BACK;
	else
		return SIDE_ON;
}

inline int VPlane::GetPointSideExact(const Vec3& vPoint) const
{
	return DistTo(vPoint) > 0.0f ? SIDE_FRONT : SIDE_BACK;
}


// BUGBUG: This should either simply use the implementation in mathlib or cease to exist.
// mathlib implementation is much more efficient.  Check to see that VPlane isn't used in
// performance critical code.
inline int VPlane::BoxOnPlaneSide(const Vec3& vMin, const Vec3& vMax) const
{
	int i, firstSide, side;
	Vec3 vPoints[8] =
	{
		{ vMin.x, vMin.y, vMin.z },
		{ vMin.x, vMin.y, vMax.z },
		{ vMin.x, vMax.y, vMax.z },
		{ vMin.x, vMax.y, vMin.z },

		{ vMax.x, vMin.y, vMin.z },
		{ vMax.x, vMin.y, vMax.z },
		{ vMax.x, vMax.y, vMax.z },
		{ vMax.x, vMax.y, vMin.z },
	};

	firstSide = GetPointSideExact(vPoints[0]);
	for (i = 1; i < 8; i++)
	{
		side = GetPointSideExact(vPoints[i]);

		// Does the box cross the plane?
		if (side != firstSide)
			return SIDE_ON;
	}

	// Ok, they're all on the same side, return that.
	return firstSide;
}