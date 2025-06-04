#pragma once
#include <cstdint>
template<uint32_t N>
class Frame
{
public:
	float mValue[N];
	float mInTangentSlope[N];//用于三次曲线插值的入切线斜率
	float mOutTangentSlope[N];//用于三次曲线插值的出切线
	float mTime;//时刻

};

typedef Frame<1> ScalarFrame;
typedef Frame<3> VectorFrame;
typedef Frame<4> QuaternionFrame;