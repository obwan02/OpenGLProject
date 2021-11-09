#pragma once
#include <math.h>
#include <stdint.h>

#include "core.h"

namespace ogl {

	template<typename T>
	struct Vector2 {
		T x, y;

		Vector2 operator+(const Vector2& other) const { return Vector2{ x + other.x, y + other.y }; }
		Vector2 operator-(const Vector2& other) const { return Vector2{ x - other.x, y - other.y }; }
		Vector2 operator*(const Vector2& other) const { return Vector2{ x * other.x, y * other.y }; }
		Vector2 operator/(const Vector2& other) const { return Vector2{ x / other.x, y / other.y }; }
		Vector2 operator+(const T& a) const { return Vector2{ x + a, y + a }; }
		Vector2 operator-(const T& a) const { return Vector2{ x - a, y - a }; }
		Vector2 operator*(const T& a) const { return Vector2{ x * a, y * a }; }
		Vector2 operator/(const T& a) const { return Vector2{ x / a, y / a }; }

		Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
		Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
		Vector2& operator*=(const Vector2& other) { x *= other.x; y *= other.y; return *this; }
		Vector2& operator/=(const Vector2& other) { x /= other.x; y /= other.y; return *this; }
		Vector2& operator+=(const T& a) { x += a; y += a; return *this; }
		Vector2& operator-=(const T& a) { x -= a; y -= a; return *this; }
		Vector2& operator*=(const T& a) { x *= a; y *= a; return *this; }
		Vector2& operator/=(const T& a) { x /= a; y /= a; return *this; }
		
		T& operator[](size_t index) { return *(reinterpret_cast<T*>(this) + index); }

	};

	template<typename T>
	struct Vector3 {
		T x, y, z;

		Vector3 operator+(const Vector3& other) const { return Vector3{ x + other.x, y + other.y, z + other.z }; }
		Vector3 operator-(const Vector3& other) const { return Vector3{ x - other.x, y - other.y, z - other.z }; }
		Vector3 operator*(const Vector3& other) const { return Vector3{ x * other.x, y * other.y, z * other.z }; }
		Vector3 operator/(const Vector3& other) const { return Vector3{ x / other.x, y / other.y, z / other.z }; }
		Vector3 operator+(const T& a) const { return Vector3{ x + a, y + a, z + a }; }
		Vector3 operator-(const T& a) const { return Vector3{ x - a, y - a, z - a }; }
		Vector3 operator*(const T& a) const { return Vector3{ x * a, y * a, z * a }; }
		Vector3 operator/(const T& a) const { return Vector3{ x / a, y / a, z / a }; }

		Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
		Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
		Vector3& operator*=(const Vector3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
		Vector3& operator/=(const Vector3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
		Vector3& operator+=(const T& a) { x += a; y += a; z += a; return *this; }
		Vector3& operator-=(const T& a) { x -= a; y -= a; z -= a; return *this; }
		Vector3& operator*=(const T& a) { x *= a; y *= a; z *= a; return *this; }
		Vector3& operator/=(const T& a) { x /= a; y /= a; z /= a; return *this; }

		T& operator[](size_t index) { return *(reinterpret_cast<T*>(this) + index); }
	};

	template<typename T>
	struct Vector4 {
		T x, y, z, w;

		Vector4 operator+(const Vector4& other) const { return Vector4{ x + other.x, y + other.y, z + other.z, w + other.w }; }
		Vector4 operator-(const Vector4& other) const { return Vector4{ x - other.x, y - other.y, z - other.z, w - other.w }; }
		Vector4 operator*(const Vector4& other) const { return Vector4{ x * other.x, y * other.y, z * other.z, w * other.w }; }
		Vector4 operator/(const Vector4& other) const { return Vector4{ x / other.x, y / other.y, z / other.z, w / other.w }; }
		Vector4 operator+(const T& a) const { return Vector4{ x + a, y + a, z + a, w + a }; }
		Vector4 operator-(const T& a) const { return Vector4{ x - a, y - a, z - a, w - a }; }
		Vector4 operator*(const T& a) const { return Vector4{ x * a, y * a, z * a, w * a }; }
		Vector4 operator/(const T& a) const { return Vector4{ x / a, y / a, z / a, z / a }; }

		Vector4& operator+=(const Vector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
		Vector4& operator-=(const Vector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
		Vector4& operator*=(const Vector4& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
		Vector4& operator/=(const Vector4& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
		Vector4& operator+=(const T& a) { x += a; y += a; z += a; w += a; return *this; }
		Vector4& operator-=(const T& a) { x -= a; y -= a; z -= a; w -= a; return *this; }
		Vector4& operator*=(const T& a) { x *= a; y *= a; z *= a; w *= a; return *this; }
		Vector4& operator/=(const T& a) { x /= a; y /= a; z /= a; w /= a; return *this; }

		T& operator[](size_t index) { return *(reinterpret_cast<T*>(this) + index); }
	};

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;
	using Vector4f = Vector4<float>;

	using Vector2i = Vector2<int>;
	using Vector3i = Vector3<int>;
	using Vector4i = Vector4<int>;

	using Vector2i32 = Vector2<int32_t>;
	using Vector3i32 = Vector3<int32_t>;
	using Vector4i32 = Vector4<int32_t>;

	using Vector2ui32 = Vector2<uint32_t>;
	using Vector3ui32 = Vector3<uint32_t>;
	using Vector4ui32 = Vector4<uint32_t>;

	using Vector2i64 = Vector2<int64_t>;
	using Vector3i64 = Vector3<int64_t>;
	using Vector4i64 = Vector4<int64_t>;

	using Vector2ui64 = Vector2<uint64_t>;
	using Vector3ui64 = Vector3<uint64_t>;
	using Vector4ui64 = Vector4<uint64_t>;
}
