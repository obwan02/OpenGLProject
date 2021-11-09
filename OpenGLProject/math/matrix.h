#pragma once

#include <cstdlib>

#include "core.h"
#include "vector.h"


namespace ogl {

	template<typename T>
	struct Matrix4 {
		T data[16];

		Matrix4<T> mat_mult(const Matrix4<T>& other) const {
			Matrix4<T> result{};
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.data[i * 4 + j] = 
						data[i * 4 + 0] * other.data[0 * 4 + i] +
						data[i * 4 + 1] * other.data[1 * 4 + i] +
						data[i * 4 + 2] * other.data[2 * 4 + i] +
						data[i * 4 + 3] * other.data[3 * 4 + i];
				}
			}

			return result;
		}

		Vector4<T> vec_mult(const Vector4<T>& other) const {
			Vector4<T> result{};
			result.x = data[0]  * other.x + data[1]  * other.y + data[2]  * other.z + data[3]  * other.w;
			result.y = data[4]  * other.x + data[5]  * other.y + data[6]  * other.z + data[7]  * other.w;
			result.z = data[8]  * other.x + data[9]  * other.y + data[10] * other.z + data[11] * other.w;
			result.w = data[12] * other.x + data[13] * other.y + data[14] * other.z + data[15] * other.w;
			return result;
		}

		static Matrix4<T> Ident(const T& diag) {
			Matrix4<T> result{};	
			result.data[0]  = diag;
			result.data[5]  = diag;
			result.data[10] = diag;
			result.data[15] = diag;
		}

		static Matrix4<T> Ortho(const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far) { 
			Matrix4<T> result{};
			result.data[0]  = T{ 2} / (right - left);
			result.data[5]  = T{ 2} / (top - bottom);
			result.data[10] = T{-2} / (far - near);
			result.data[12] = -(right + left)/(right - left);
			result.data[13] = -(top + bottom)/(top - bottom);
			result.data[14] = -(far + near)/(far - near);
			result.data[15] = T{1};
			return result;
		}
	};

	using Matrix4f = Matrix4<float>;
	using Matrix3f = std::nullptr_t;
}
