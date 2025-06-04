#pragma once
#include "Quaternion.hpp"
namespace RealSix
{
    template <typename T>
    class Transform;

    //reference:https://optsolution.github.io/archives/30055.html
    template <typename T>
    class DualQuaternion
    {
    public:
        union
        {
            struct
            {
                Quaternion<T> real;
                Quaternion<T> dual;
            };
            struct
            {
                T v[8];
            };
        };

        DualQuaternion();
        DualQuaternion(const DualQuaternion<T> &dq);
        DualQuaternion(const Quaternion<T> &r, const Quaternion<T> &d);
        ~DualQuaternion();

        DualQuaternion<T> operator=(const DualQuaternion<T> &dq);

        static T Dot(const DualQuaternion<T> &left, const DualQuaternion<T> &right);
        static DualQuaternion<T> Conjugate(const DualQuaternion<T> &dq);
        static DualQuaternion<T> Normalize(const DualQuaternion<T> &dq);

        static Transform<T> DualQuaternionToTransform(const DualQuaternion<T> &dq);
    };

    typedef DualQuaternion<float> DualQuaternionf;
    typedef DualQuaternion<double> DualQuaterniond;
    typedef DualQuaternion<int32_t> DualQuaternioni32;
    typedef DualQuaternion<uint32_t> DualQuaternionu32;
    typedef DualQuaternion<int16_t> DualQuaternioni16;
    typedef DualQuaternion<uint16_t> DualQuaternionu16;
    typedef DualQuaternion<int8_t> DualQuaternioni8;
    typedef DualQuaternion<uint8_t> DualQuaternionu8;

    template <typename T>
    inline DualQuaternion<T>::DualQuaternion(/* args */)
        : real(Quaternion<T>()), dual(Quaternion<T>(0.0f, 0.0f, 0.0f, 0.0f))
    {
    }

    template <typename T>
    DualQuaternion<T>::DualQuaternion(const DualQuaternion<T> &dq)
    {
        this->real = dq.real;
        this->dual = dq.dual;
    }

    template <typename T>
    inline DualQuaternion<T>::DualQuaternion(const Quaternion<T> &r, const Quaternion<T> &d)
        : real(r), dual(d)
    {
    }
    template <typename T>
    inline DualQuaternion<T>::~DualQuaternion()
    {
    }

    template <typename T>
    DualQuaternion<T> DualQuaternion<T>::operator=(const DualQuaternion<T> &dq)
    {
        this->real = dq.real;
        this->dual = dq.dual;
        return *this;
    }

    template <typename T>
    inline DualQuaternion<T> operator+(const DualQuaternion<T> &left, const DualQuaternion<T> &right)
    {
        return DualQuaternion<T>(left.real + right.real, left.dual + right.dual);
    }

    template <typename T>
    inline DualQuaternion<T> operator*(const DualQuaternion<T> &left, T value)
    {
        return DualQuaternion<T>(left.real * value, left.dual * value);
    }

    //双四元数的乘法从左到右,与矩阵和四元数相反
    template <typename T>
    inline DualQuaternion<T> operator*(const DualQuaternion<T> &left, const DualQuaternion<T> &right)
    {
        DualQuaternion<T> lhs = DualQuaternion<T>::Normalize(left);
        DualQuaternion<T> rhs = DualQuaternion<T>::Normalize(right);
        return DualQuaternion<T>(lhs.real * rhs.real, lhs.real * rhs.dual + lhs.dual * rhs.real);
    }

    template <typename T>
    inline bool operator==(const DualQuaternion<T> &left, const DualQuaternion<T> &right)
    {
        return left.real == right.real && left.dual == right.dual;
    }

    template <typename T>
    inline bool operator!=(const DualQuaternion<T> &left, const DualQuaternion<T> &right)
    {
        return !(left == right);
    }

    template <typename T>
    inline T Dot(const DualQuaternion<T> &left, const DualQuaternion<T> &right)
    {
        return Quaternion<T>::Dot(left.real, right.real);
    }
    template <typename T>
    inline DualQuaternion<T> DualQuaternion<T>::Conjugate(const DualQuaternion<T> &dq)
    {
        return DualQuaternion<T>(Quaternion<T>::Conjugate(dq.real), Quaternion<T>::Conjugate(dq.dual));
    }
    template <typename T>
    inline DualQuaternion<T> DualQuaternion<T>::Normalize(const DualQuaternion<T> &dq)
    {
        T magSq = Quaternion<T>::Dot(dq.real, dq.real);
        if (magSq < std::numeric_limits<float>::epsilon())
            return DualQuaternion<T>();

        T invMag = 1.0f / Math::Sqrt(magSq);

        return DualQuaternion<T>(dq.real * invMag, dq.dual * invMag);
    }
    template <typename T>
    inline Transform<T> DualQuaternion<T>::DualQuaternionToTransform(const DualQuaternion<T> &dq)
    {
        Transform<T> result;
        result.rotation = dq.real;
        Quaternion<T> d = Quaternion<T>::Conjuate(dq.real) * (dq.dual * 2.0f);
        result.position = d.vec;
        return result;
    }
}