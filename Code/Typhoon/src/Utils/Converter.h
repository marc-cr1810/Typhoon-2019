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

static int BytesToInt(std::vector<Ty_uint8_t> bytes)
{
	int result = 0;
	for (unsigned n = 0; n < bytes.size(); n++)
		result = (result << 8) + bytes[n];
	return result;
}

static float BytesToFloat(std::vector<Ty_uint8_t> bytes)
{
	float member = 0.0;

	member += (float)(bytes[0] << 24);
	member += (float)(bytes[1] << 16);
	member += (float)(bytes[2] << 8);
	member += (float)(bytes[3]);

	return member;
}

#endif