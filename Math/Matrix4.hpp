#pragma once
#include <xmmintrin.h>
#include <cstdint>
#include <array>
#include <iostream>
#include <cassert>
#include "Math.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"
#include "Core/Config.hpp"
#include "Core/Marco.hpp"
#include "Core/Logger.hpp"
namespace RealSix
{
	template <typename T>
	class Vector3;

	template <typename T>
	class Vector4;

	template <typename T>
	class Matrix3;

	template <typename T>
	class Transform;

	template <typename T>
	class Matrix4
	{
	public:
		union
		{
			struct
			{
				std::array<__m128, 4> col;
			};
			struct
			{
				T element00, element10, element20, element30,
					element01, element11, element21, element31,
					element02, element12, element22, element32,
					element03, element13, element23, element33;
			};
			struct
			{
				std::array<T, 16> elements;
			};
			struct
			{
				std::array<Vector4<T>, 4> columns;
			};
		};

		Matrix4();
		Matrix4(const T &value);
		Matrix4(const Vector4<T> &diagonal);
		Matrix4(const T &d00, const T &d11, const T &d22, const T &d33);
		Matrix4(const T &e00, const T &e10, const T &e20, const T &e30,
				const T &e01, const T &e11, const T &e21, const T &e31,
				const T &e02, const T &e12, const T &e22, const T &e32,
				const T &e03, const T &e13, const T &e23, const T &e33);

		void Set(const Vector4<T> &diagonal);
		void Set(const T &d00, const T &d11, const T &d22, const T &d33);
		void Set(const T &e00, const T &e10, const T &e20, const T &e30,
				 const T &e01, const T &e11, const T &e21, const T &e31,
				 const T &e02, const T &e12, const T &e22, const T &e32,
				 const T &e03, const T &e13, const T &e23, const T &e33);

		template <typename T2>
		Matrix4<T> &operator+=(const Matrix4<T2> &right);
		template <typename T2>
		Matrix4<T> &operator-=(const Matrix4<T2> &right);
		template <typename T2>
		Matrix4<T> &operator*=(const T2 &value);
		template <typename T2>
		Vector4<T> &operator[](T2 idx);

		static Matrix4<T> Translate(const Vector3<T> &position);
		static Matrix4<T> Rotate(const Vector3<T> &axis, const T &radian);
		static Matrix4<T> Scale(const T &factor);
		static Matrix4<T> Scale(const Vector3<T> &factor);
		static Matrix4<T> Transpose(const Matrix4<T> &right);
		static Matrix4<T> Inverse(const Matrix4<T> &right);
		static T Determinant(const Matrix4<T> &right);
		static Matrix4<T> Adjoint(const Matrix4<T> &right);
		static Matrix4<T> OrthoGraphic(const T &left, const T &right, const T &top, const T &bottom, const T &znear, const T &zfar);
		static Matrix4<T> Perspective(const T &fov_radian, const T &aspect, const T &znear, const T &zfar);
		static Matrix4<T> LookAt(const Vector3<T> &position, const Vector3<T> &target, const Vector3<T> &up);
		static Matrix3<T> ToMatrix3(const Matrix4<T> &matrix);
		static Quaternion<T> ToQuaternion(const Matrix4<T> &matrix);
		static Transform3<T> ToTransform(const Matrix4<T> &matrix);

		static const Matrix4<T> IDENTITY;

	private:
		static Matrix4<T> GLOrthoGraphic(const T &left, const T &right, const T &top, const T &bottom, const T &znear, const T &zfar);
		static Matrix4<T> VKOrthoGraphic(const T &left, const T &right, const T &top, const T &bottom, const T &znear, const T &zfar);
		static Matrix4<T> GLPerspective(const T &fov_radian, const T &aspect, const T &znear, const T &zfar);
		static Matrix4<T> VKPerspective(const T &fov_radian, const T &aspect, const T &znear, const T &zfar);
	};

	typedef Matrix4<float> Matrix4f;
	typedef Matrix4<double> Matrix4d;
	typedef Matrix4<int32_t> Matrix4i32;
	typedef Matrix4<uint32_t> Matrix4u32;
	typedef Matrix4<int16_t> Matrix4i16;
	typedef Matrix4<uint16_t> Matrix4u16;
	typedef Matrix4<int8_t> Matrix4i8;
	typedef Matrix4<uint8_t> Matrix4u8;

	template <typename T>
	const Matrix4<T> Matrix4<T>::IDENTITY = Matrix4<T>();

	template <typename T>
	inline T Matrix4<T>::Determinant(const Matrix4<T> &right)
	{
		return right.elements[0] * right.elements[5] * right.elements[10] * right.elements[15] -
			   right.elements[4] * right.elements[9] * right.elements[14] * right.elements[3] +
			   right.elements[8] * right.elements[13] * right.elements[2] * right.elements[7] -
			   right.elements[12] * right.elements[1] * right.elements[6] * right.elements[11] +
			   right.elements[12] * right.elements[9] * right.elements[6] * right.elements[3] -
			   right.elements[0] * right.elements[12] * right.elements[10] * right.elements[7] +
			   right.elements[4] * right.elements[1] * right.elements[14] * right.elements[11] -
			   right.elements[8] * right.elements[5] * right.elements[2] * right.elements[15] +
			   right.elements[4] * right.elements[9] * right.elements[2] * right.elements[15] -
			   right.elements[8] * right.elements[1] * right.elements[14] * right.elements[7] +
			   right.elements[0] * right.elements[13] * right.elements[6] * right.elements[11] -
			   right.elements[12] * right.elements[5] * right.elements[10] * right.elements[3] +
			   right.elements[12] * right.elements[1] * right.elements[10] * right.elements[7] -
			   right.elements[4] * right.elements[13] * right.elements[2] * right.elements[11] +
			   right.elements[8] * right.elements[5] * right.elements[14] * right.elements[3] -
			   right.elements[0] * right.elements[9] * right.elements[6] * right.elements[15] +
			   right.elements[8] * right.elements[1] * right.elements[6] * right.elements[15] -
			   right.elements[0] * right.elements[5] * right.elements[14] * right.elements[11] +
			   right.elements[4] * right.elements[13] * right.elements[10] * right.elements[3] -
			   right.elements[12] * right.elements[9] * right.elements[2] * right.elements[7] +
			   right.elements[12] * right.elements[5] * right.elements[2] * right.elements[11] -
			   right.elements[8] * right.elements[13] * right.elements[6] * right.elements[3] +
			   right.elements[0] * right.elements[9] * right.elements[14] * right.elements[7] -
			   right.elements[4] * right.elements[1] * right.elements[10] * right.elements[15];
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Adjoint(const Matrix4<T> &right)
	{
		Matrix4<T> adj;

		Matrix3<T> tmpMat;

		tmpMat.elements[0] = right.elements[5];
		tmpMat.elements[1] = right.elements[6];
		tmpMat.elements[2] = right.elements[7];
		tmpMat.elements[3] = right.elements[9];
		tmpMat.elements[4] = right.elements[10];
		tmpMat.elements[5] = right.elements[11];
		tmpMat.elements[6] = right.elements[13];
		tmpMat.elements[7] = right.elements[14];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[0] = Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[4];
		tmpMat.elements[1] = right.elements[6];
		tmpMat.elements[2] = right.elements[7];
		tmpMat.elements[3] = right.elements[8];
		tmpMat.elements[4] = right.elements[10];
		tmpMat.elements[5] = right.elements[11];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[14];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[1] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[4];
		tmpMat.elements[1] = right.elements[5];
		tmpMat.elements[2] = right.elements[7];
		tmpMat.elements[3] = right.elements[8];
		tmpMat.elements[4] = right.elements[9];
		tmpMat.elements[5] = right.elements[11];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[13];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[2] = Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[4];
		tmpMat.elements[1] = right.elements[5];
		tmpMat.elements[2] = right.elements[6];
		tmpMat.elements[3] = right.elements[8];
		tmpMat.elements[4] = right.elements[9];
		tmpMat.elements[5] = right.elements[10];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[13];
		tmpMat.elements[8] = right.elements[14];
		adj.elements[3] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[1];
		tmpMat.elements[1] = right.elements[2];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[9];
		tmpMat.elements[4] = right.elements[10];
		tmpMat.elements[5] = right.elements[11];
		tmpMat.elements[6] = right.elements[13];
		tmpMat.elements[7] = right.elements[14];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[4] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[2];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[8];
		tmpMat.elements[4] = right.elements[10];
		tmpMat.elements[5] = right.elements[11];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[14];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[5] = Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[1];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[8];
		tmpMat.elements[4] = right.elements[9];
		tmpMat.elements[5] = right.elements[11];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[13];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[6] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[1];
		tmpMat.elements[2] = right.elements[2];
		tmpMat.elements[3] = right.elements[8];
		tmpMat.elements[4] = right.elements[9];
		tmpMat.elements[5] = right.elements[10];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[13];
		tmpMat.elements[8] = right.elements[14];
		adj.elements[7] = Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[1];
		tmpMat.elements[1] = right.elements[2];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[5];
		tmpMat.elements[4] = right.elements[6];
		tmpMat.elements[5] = right.elements[7];
		tmpMat.elements[6] = right.elements[13];
		tmpMat.elements[7] = right.elements[14];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[8] = Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[2];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[4];
		tmpMat.elements[4] = right.elements[6];
		tmpMat.elements[5] = right.elements[7];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[14];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[9] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[1];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[4];
		tmpMat.elements[4] = right.elements[5];
		tmpMat.elements[5] = right.elements[7];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[13];
		tmpMat.elements[8] = right.elements[15];
		adj.elements[10] = Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[1];
		tmpMat.elements[2] = right.elements[2];
		tmpMat.elements[3] = right.elements[4];
		tmpMat.elements[4] = right.elements[5];
		tmpMat.elements[5] = right.elements[6];
		tmpMat.elements[6] = right.elements[12];
		tmpMat.elements[7] = right.elements[13];
		tmpMat.elements[8] = right.elements[14];
		adj.elements[11] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[1];
		tmpMat.elements[1] = right.elements[2];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[5];
		tmpMat.elements[4] = right.elements[6];
		tmpMat.elements[5] = right.elements[7];
		tmpMat.elements[6] = right.elements[9];
		tmpMat.elements[7] = right.elements[10];
		tmpMat.elements[8] = right.elements[11];
		adj.elements[12] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[2];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[4];
		tmpMat.elements[4] = right.elements[6];
		tmpMat.elements[5] = right.elements[7];
		tmpMat.elements[6] = right.elements[8];
		tmpMat.elements[7] = right.elements[10];
		tmpMat.elements[8] = right.elements[11];
		adj.elements[13] = Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[1];
		tmpMat.elements[2] = right.elements[3];
		tmpMat.elements[3] = right.elements[4];
		tmpMat.elements[4] = right.elements[5];
		tmpMat.elements[5] = right.elements[7];
		tmpMat.elements[6] = right.elements[8];
		tmpMat.elements[7] = right.elements[9];
		tmpMat.elements[8] = right.elements[11];
		adj.elements[14] = -Matrix3<T>::Determinant(tmpMat);

		tmpMat.elements[0] = right.elements[0];
		tmpMat.elements[1] = right.elements[1];
		tmpMat.elements[2] = right.elements[2];
		tmpMat.elements[3] = right.elements[4];
		tmpMat.elements[4] = right.elements[5];
		tmpMat.elements[5] = right.elements[6];
		tmpMat.elements[6] = right.elements[8];
		tmpMat.elements[7] = right.elements[9];
		tmpMat.elements[8] = right.elements[10];
		adj.elements[15] = Matrix3<T>::Determinant(tmpMat);

		return adj;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::OrthoGraphic(const T &left, const T &right, const T &top, const T &bottom, const T &znear, const T &zfar)
	{
		const GfxConfig &gfxConfig = GfxConfig::GetInstance();
		switch (gfxConfig.GetBackend())
		{
		case GfxBackend::VULKAN:
		{
			return VKOrthoGraphic(left, right, top, bottom, znear, zfar);
		}
		case GfxBackend::D3D12:
			REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
			break;
		default:
			REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
			break;
		}

		REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
		return Matrix4<T>(); // for avoiding compiler warning
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::GLOrthoGraphic(const T &left, const T &right, const T &top, const T &bottom, const T &znear, const T &zfar)
	{
		Matrix4<T> tmp;
		tmp.elements[0] = 2 / (right - left);
		tmp.elements[5] = 2 / (top - bottom);
		tmp.elements[10] = 2 / (znear - zfar);
		tmp.elements[12] = (left + right) / (left - right);
		tmp.elements[13] = (bottom + top) / (bottom - top);
		tmp.elements[14] = (znear + zfar) / (znear - zfar);
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::VKOrthoGraphic(const T &left, const T &right, const T &top, const T &bottom, const T &znear, const T &zfar)
	{
		Matrix4<T> tmp;
		tmp.elements[0] = 2 / (right - left);
		tmp.elements[5] = 2 / (bottom - top);
		tmp.elements[10] = 1 / (znear - zfar);
		tmp.elements[12] = (left + right) / (left - right);
		tmp.elements[13] = -(bottom + top) / (bottom - top);
		tmp.elements[14] = (znear) / (znear - zfar);
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Perspective(const T &fov_radian, const T &aspect, const T &znear, const T &zfar)
	{
		const GfxConfig &gfxConfig = GfxConfig::GetInstance();
		switch (gfxConfig.GetBackend())
		{
		case GfxBackend::VULKAN:
		{
			return VKPerspective(fov_radian, aspect, znear, zfar);
		}
		case GfxBackend::D3D12:
			REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
			break;
		default:
			REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
			break;
		}

		REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
		return Matrix4<T>(); // for avoiding compiler warning
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::GLPerspective(const T &fov_radian, const T &aspect, const T &znear, const T &zfar)
	{
		T cotFov = Math::Cot(fov_radian / 2);

		Matrix4<T> tmp(static_cast<T>(0.0f));
		tmp.elements[0] = cotFov / aspect;
		tmp.elements[5] = cotFov;
		tmp.elements[10] = (znear + zfar) / (znear - zfar);
		tmp.elements[11] = static_cast<T>(-1.0f);
		tmp.elements[14] = 2 * znear * zfar / (znear - zfar);

		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::VKPerspective(const T &fov_radian, const T &aspect, const T &znear, const T &zfar)
	{
		float f = Math::Cot(fov_radian / 2);

		Matrix4<T> tmp(static_cast<T>(0.0f));
		tmp.elements[0] = f / aspect;
		tmp.elements[5] = f;
		tmp.elements[10] = zfar / (znear - zfar);
		tmp.elements[11] = static_cast<T>(-1.0f);
		tmp.elements[14] = (znear * zfar) / (znear - zfar);
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::LookAt(const Vector3<T> &position, const Vector3<T> &target, const Vector3<T> &up)
	{
		Vector3<T> axisZ = Vector3<T>::Normalize(position - target);
		Vector3<T> axisX = Vector3<T>::Normalize(Vector3<T>::Cross(up, axisZ));
		Vector3<T> axisY = Vector3<T>::Normalize(Vector3<T>::Cross(axisZ, axisX));

		Matrix4<T> rotPart;
		rotPart.elements[0] = axisX.x;
		rotPart.elements[1] = axisY.x;
		rotPart.elements[2] = axisZ.x;

		rotPart.elements[4] = axisX.y;
		rotPart.elements[5] = axisY.y;
		rotPart.elements[6] = axisZ.y;

		rotPart.elements[8] = axisX.z;
		rotPart.elements[9] = axisY.z;
		rotPart.elements[10] = axisZ.z;

		Matrix4<T> transPart;
		transPart.elements[12] = -position.x;
		transPart.elements[13] = -position.y;
		transPart.elements[14] = -position.z;

		return rotPart * transPart;
	}

	template <typename T>
	inline Matrix3<T> Matrix4<T>::ToMatrix3(const Matrix4<T> &matrix)
	{
		Matrix3<T> tmp;
		tmp.elements[0] = matrix.elements[0];
		tmp.elements[1] = matrix.elements[1];
		tmp.elements[2] = matrix.elements[2];
		tmp.elements[3] = matrix.elements[4];
		tmp.elements[4] = matrix.elements[5];
		tmp.elements[5] = matrix.elements[6];
		tmp.elements[6] = matrix.elements[8];
		tmp.elements[7] = matrix.elements[9];
		tmp.elements[8] = matrix.elements[10];
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> operator+(const Matrix4<T> &left, const Matrix4<T> &right)
	{
		Matrix4<T> tmp;
		for (uint8_t i = 0; i < 4; ++i)
			tmp.col[i] = _mm_add_ps(left.col[i], right.col[i]);
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> operator-(const Matrix4<T> &left, const Matrix4<T> &right)
	{
		Matrix4<T> tmp;
		for (uint8_t i = 0; i < 4; ++i)
			tmp.col[i] = _mm_sub_ps(left.col[i], right.col[i]);
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> operator*(const Matrix4<T> &left, const Matrix4<T> &right)
	{
		Matrix4<T> tmp;

		tmp.elements[0] = left.elements[0] * right.elements[0] + left.elements[4] * right.elements[1] + left.elements[8] * right.elements[2] + left.elements[12] * right.elements[3];
		tmp.elements[1] = left.elements[1] * right.elements[0] + left.elements[5] * right.elements[1] + left.elements[9] * right.elements[2] + left.elements[13] * right.elements[3];
		tmp.elements[2] = left.elements[2] * right.elements[0] + left.elements[6] * right.elements[1] + left.elements[10] * right.elements[2] + left.elements[14] * right.elements[3];
		tmp.elements[3] = left.elements[3] * right.elements[0] + left.elements[7] * right.elements[1] + left.elements[11] * right.elements[2] + left.elements[15] * right.elements[3];

		tmp.elements[4] = left.elements[0] * right.elements[4] + left.elements[4] * right.elements[5] + left.elements[8] * right.elements[6] + left.elements[12] * right.elements[7];
		tmp.elements[5] = left.elements[1] * right.elements[4] + left.elements[5] * right.elements[5] + left.elements[9] * right.elements[6] + left.elements[13] * right.elements[7];
		tmp.elements[6] = left.elements[2] * right.elements[4] + left.elements[6] * right.elements[5] + left.elements[10] * right.elements[6] + left.elements[14] * right.elements[7];
		tmp.elements[7] = left.elements[3] * right.elements[4] + left.elements[7] * right.elements[5] + left.elements[11] * right.elements[6] + left.elements[15] * right.elements[7];

		tmp.elements[8] = left.elements[0] * right.elements[8] + left.elements[4] * right.elements[9] + left.elements[8] * right.elements[10] + left.elements[12] * right.elements[11];
		tmp.elements[9] = left.elements[1] * right.elements[8] + left.elements[5] * right.elements[9] + left.elements[9] * right.elements[10] + left.elements[13] * right.elements[11];
		tmp.elements[10] = left.elements[2] * right.elements[8] + left.elements[6] * right.elements[9] + left.elements[10] * right.elements[10] + left.elements[14] * right.elements[11];
		tmp.elements[11] = left.elements[3] * right.elements[8] + left.elements[7] * right.elements[9] + left.elements[11] * right.elements[10] + left.elements[15] * right.elements[11];

		tmp.elements[12] = left.elements[0] * right.elements[12] + left.elements[4] * right.elements[13] + left.elements[8] * right.elements[14] + left.elements[12] * right.elements[15];
		tmp.elements[13] = left.elements[1] * right.elements[12] + left.elements[5] * right.elements[13] + left.elements[9] * right.elements[14] + left.elements[13] * right.elements[15];
		tmp.elements[14] = left.elements[2] * right.elements[12] + left.elements[6] * right.elements[13] + left.elements[10] * right.elements[14] + left.elements[14] * right.elements[15];
		tmp.elements[15] = left.elements[3] * right.elements[12] + left.elements[7] * right.elements[13] + left.elements[11] * right.elements[14] + left.elements[15] * right.elements[15];

		return tmp;
	}

	template <typename T>
	inline Matrix4<T> operator/(const Matrix4<T> &left, const T &value)
	{
		if (!Math::IsNearZero(value))
		{
			Matrix4<T> tmp;

			for (uint8_t i = 0; i < 4; ++i)
				tmp.col[i] = _mm_div_ps(left.col[i], _mm_set_ps1(value));
			return tmp;
		}
		return left;
	}

	template <typename T, typename T2>
	inline Matrix4<T> operator*(const T &value, const Matrix4<T2> &right)
	{
		Matrix4<T> tmp;
		for (uint8_t i = 0; i < 4; ++i)
			tmp.col[i] = _mm_mul_ps(right.col[i], _mm_set_ps1(value));
		return tmp;
	}

	template <typename T, typename T2>
	inline Matrix4<T> operator*(const Matrix4<T> &left, const T2 &value)
	{
		return value * left;
	}

	template <typename T>
	template <typename T2>
	inline Matrix4<T> &Matrix4<T>::operator+=(const Matrix4<T2> &right)
	{

		*this = *this + right;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix4<T> &Matrix4<T>::operator-=(const Matrix4<T2> &right)
	{

		*this = this - right;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix4<T> &Matrix4<T>::operator*=(const T2 &value)
	{

		*this = *this * value;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Vector4<T> &Matrix4<T>::operator[](T2 idx)
	{
		assert(idx < 4);
		return columns[idx];
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Translate(const Vector3<T> &position)
	{
		Matrix4<T> tmp;
		tmp.col[3] = _mm_set_ps(static_cast<T>(1.0f), position.z, position.y, position.x);
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Rotate(const Vector3<T> &axis, const T &radian)
	{
		Matrix4<T> tmp;
		T radian_cos = Math::Cos(radian);
		T radian_sin = Math::Sin(radian);
		T tmpNum = static_cast<T>(1.0f) - radian_cos;

		tmp.elements[0] = axis.x * axis.x * tmpNum + radian_cos;
		tmp.elements[1] = axis.y * axis.x * tmpNum + axis.z * radian_sin;
		tmp.elements[2] = axis.x * axis.z * tmpNum - axis.y * radian_sin;

		tmp.elements[4] = axis.y * axis.x * tmpNum - axis.z * radian_sin;
		tmp.elements[5] = axis.y * axis.y * tmpNum + radian_cos;
		tmp.elements[6] = axis.z * axis.y * tmpNum + axis.x * radian_sin;

		tmp.elements[8] = axis.x * axis.z * tmpNum + axis.y * radian_sin;
		tmp.elements[9] = axis.z * axis.y * tmpNum - axis.x * radian_sin;
		tmp.elements[10] = axis.z * axis.z * tmpNum + radian_cos;

		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Scale(const T &factor)
	{
		Matrix4<T> tmp;
		tmp.elements[0] = factor;
		tmp.elements[5] = factor;
		tmp.elements[10] = factor;
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Scale(const Vector3<T> &factor)
	{
		Matrix4<T> tmp;
		tmp.elements[0] = factor.x;
		tmp.elements[5] = factor.y;
		tmp.elements[10] = factor.z;
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Transpose(const Matrix4<T> &right)
	{
		Matrix4<T> tmp;
		tmp.col[0] = _mm_set_ps(right.elements[12], right.elements[8], right.elements[4], right.elements[0]);
		tmp.col[1] = _mm_set_ps(right.elements[13], right.elements[9], right.elements[5], right.elements[1]);
		tmp.col[2] = _mm_set_ps(right.elements[14], right.elements[10], right.elements[6], right.elements[2]);
		tmp.col[3] = _mm_set_ps(right.elements[15], right.elements[11], right.elements[7], right.elements[3]);

		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix4<T>::Inverse(const Matrix4<T> &right)
	{
		return Matrix4f::Transpose(Matrix4<T>::Adjoint(right)) / Matrix4<T>::Determinant(right);
	}

	template <typename T>
	inline Matrix4<T>::Matrix4()
		: col({_mm_set_ps(static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(1.0f)),
			   _mm_set_ps(static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(1.0f), static_cast<T>(0.0f)),
			   _mm_set_ps(static_cast<T>(0.0f), static_cast<T>(1.0f), static_cast<T>(0.0f), static_cast<T>(0.0f)),
			   _mm_set_ps(static_cast<T>(1.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f))})
	{
	}

	template <typename T>
	inline Matrix4<T>::Matrix4(const T &value)
		: col({_mm_set_ps(static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), value),
			   _mm_set_ps(static_cast<T>(0.0f), static_cast<T>(0.0f), value, static_cast<T>(0.0f)),
			   _mm_set_ps(static_cast<T>(0.0f), value, static_cast<T>(0.0f), static_cast<T>(0.0f)),
			   _mm_set_ps(value, static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f))})
	{
	}

	template <typename T>
	inline Matrix4<T>::Matrix4(const Vector4<T> &diagonal)
		: col({_mm_set_ps1(static_cast<T>(0.0f)), _mm_set_ps1(static_cast<T>(0.0f)), _mm_set_ps1(static_cast<T>(0.0f)), _mm_set_ps1(static_cast<T>(0.0f))})
	{
		Set(diagonal);
	}

	template <typename T>
	inline Matrix4<T>::Matrix4(const T &d00, const T &d11, const T &d22, const T &d33)
		: col({_mm_set_ps1(static_cast<T>(0.0f)), _mm_set_ps1(static_cast<T>(0.0f)), _mm_set_ps1(static_cast<T>(0.0f)), _mm_set_ps1(static_cast<T>(0.0f))})
	{
		Set(d00, d11, d22, d33);
	}

	template <typename T>
	inline Matrix4<T>::Matrix4(const T &e00, const T &e10, const T &e20, const T &e30, const T &e01, const T &e11, const T &e21, const T &e31, const T &e02, const T &e12, const T &e22, const T &e32, const T &e03, const T &e13, const T &e23, const T &e33)
		: col({_mm_set_ps(e30, e20, e10, e00),
			   _mm_set_ps(e31, e21, e11, e01),
			   _mm_set_ps(e32, e22, e12, e02),
			   _mm_set_ps(e33, e23, e13, e03)})
	{
	}

	template <typename T>
	inline void Matrix4<T>::Set(const Vector4<T> &diagonal)
	{
		Set(diagonal.x, diagonal.y, diagonal.z, diagonal.w);
	}

	template <typename T>
	inline void Matrix4<T>::Set(const T &d00, const T &d11, const T &d22, const T &d33)
	{
		elements[0] = d00;
		elements[5] = d11;
		elements[10] = d22;
		elements[15] = d33;
	}

	template <typename T>
	inline void Matrix4<T>::Set(const T &e00, const T &e10, const T &e20, const T &e30, const T &e01, const T &e11, const T &e21, const T &e31, const T &e02, const T &e12, const T &e22, const T &e32, const T &e03, const T &e13, const T &e23, const T &e33)
	{
		col[0] = _mm_set_ps(e30, e20, e10, e00);
		col[1] = _mm_set_ps(e31, e21, e11, e01);
		col[2] = _mm_set_ps(e32, e22, e12, e02);
		col[2] = _mm_set_ps(e33, e23, e13, e03);
	}

	template <typename T>
	inline Quaternion<T> Matrix4<T>::ToQuaternion(const Matrix4<T> &matrix)
	{
		Vector3<T> up = Vector3<T>::Normalize(Vector3<T>(matrix.element01, matrix.element11, matrix.element21));
		Vector3<T> forward = Vector3<T>::Normalize(Vector3<T>(matrix.element02, matrix.element12, matrix.element22));
		Vector3<T> right = Vector3<T>::Cross(up, forward);
		up = Vector3<T>::Cross(forward, right);

		// Find orthonormal basis vectors
		Vector3<T> f = Vector3<T>::Normalize(forward);
		Vector3<T> u = Vector3<T>::Normalize(up);
		Vector3<T> r = Vector3<T>::Cross(u, f);
		u = Vector3<T>::Cross(f, r);

		// From world forward to object forward
		Quaternion<T> f2d = Quaternion<T>::FromTo(Vector3<T>::UNIT_Z, f);

		// what direction is the new object up?
		Vector3<T> objectUp = f2d * Vector3<T>::UNIT_Y;
		// From object up to desired up
		Quaternion<T> u2u = Quaternion<T>::FromTo(objectUp, u);

		// Rotate to forward direction first, then twist to correct up
		Quaternion<T> result = f2d * u2u;
		// Don't forget to normalize the result
		return Quaternion<T>::Normalize(result);
	}

	template <typename T>
	inline Transform3<T> Matrix4<T>::ToTransform(const Matrix4<T> &matrix)
	{
		Transform3<T> result;
		result.position = Vector3<T>(matrix.element03, matrix.element13, matrix.element23);
		result.rotation = Matrix4<T>::ToQuaternion(matrix);

		Matrix4<T> rotScaleMat = Matrix3<T>::ToMatrix4(Matrix4<T>::ToMatrix3(matrix));

		Matrix4<T> invRotMat = Quaternion<T>::ToMatrix4(Quaternion<T>::Inverse(result.rotation));

		Matrix4<T> scaleSkewMat = rotScaleMat * invRotMat;

		result.scale = Vector3<T>(scaleSkewMat.element00, scaleSkewMat.element11, scaleSkewMat.element22);

		return result;
	}
}