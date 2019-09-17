#ifndef Ty_CONVERTER_H
#define Ty_CONVERTER_H

#include "../Port.h"

static std::vector<Ty_uint8_t> StringToVector(Ty_string_t string)
{
	return std::vector<Ty_uint8_t>(string.begin(), string.end());
}

static Ty_string_t VectorToString(std::vector<Ty_uint8_t> vector)
{
	return Ty_string_t(vector.begin(), vector.end());
}

static int BytesToInt(std::vector<Ty_uint8_t> bytes, bool sign = false)
{
	int result = 0;
	for (unsigned n = 0; n < bytes.size(); n++)
		result = (result << 8) + bytes[n];
	if (sign)
	{
		if (bytes.size() == 1)
			return (Ty_int8_t)result;
		if (bytes.size() == 2)
			return (Ty_int16_t)result;
	}
	return result;
}

static float BytesToFloat(std::vector<Ty_uint8_t> bytes)
{
	union flt
	{
		float f;
		unsigned char bytes[sizeof(float)];
	};
	flt value;
	value.bytes[0] = bytes[0];
	value.bytes[1] = bytes[1];
	value.bytes[2] = bytes[2];
	value.bytes[3] = bytes[3];

	return value.f;
}

#endif