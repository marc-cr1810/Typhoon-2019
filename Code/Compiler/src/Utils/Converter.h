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

static std::vector<Ty_uint8_t> IntToBytes(int value, bool sign = true)
{
	std::vector<Ty_uint8_t> bytes;
	int size = 0;
	if (abs(value) < (sign ? (pow(2, 8) / 2) : pow(2, 8)))
		size = 1;
	else if (abs(value) < (sign ? (pow(2, 16) / 2) : pow(2, 16)))
		size = 2;
	else if (abs(value) < (sign ? (pow(2, 32) / 2) : pow(2, 32)))
		size = 4;

	int i = size * 8;
	while (i != 0) { 
		bytes.push_back(value >> (i - 8)); 
		i -= 8; 
	}

	return bytes;
}

static std::vector<Ty_uint8_t> FloatToBytes(float value)
{
	union flt
	{
		float f;
		unsigned char bytes[sizeof(float)];
	};

	std::vector<Ty_uint8_t> bytes;
	flt val;
	val.f = value;

	bytes.push_back(val.bytes[0]);
	bytes.push_back(val.bytes[1]);
	bytes.push_back(val.bytes[2]);
	bytes.push_back(val.bytes[3]);
	return bytes;
}

#endif