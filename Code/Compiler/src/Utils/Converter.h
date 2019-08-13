#ifndef Ty_CONVERTER_H
#define Ty_CONVERTER_H

#include "../Port.h"

static std::vector<Ty_uint8_t> StringToVector(Ty_string_t string)
{
	return std::vector<Ty_uint8_t>(string.begin(), string.end());
}

static std::vector<Ty_uint8_t> IntToBytes(int value)
{
	if (abs(value) < (pow(2, 8) - 1))
		return { (Ty_uint8_t)(value & 0xFF) };
	else if (abs(value) < (pow(2, 16) - 1))
		return {
				(Ty_uint8_t)((value >> 8) & 0xFF),
				(Ty_uint8_t)(value & 0xFF)
			};
	else if (abs(value) < (pow(2, 32) - 1))
		return {
				(Ty_uint8_t)((value >> 24) & 0xFF),
				(Ty_uint8_t)((value >> 16) & 0xFF),
				(Ty_uint8_t)((value >> 8) & 0xFF),
				(Ty_uint8_t)(value & 0xFF)
			};
}

static std::vector<Ty_uint8_t> FloatToBytes(float value)
{
	std::vector<Ty_uint8_t> bytes;
	unsigned long d = *(unsigned long*)&value;

	bytes.push_back(d & 0x00FF);
	bytes.push_back((d & 0xFF00) >> 8);
	bytes.push_back((d & 0xFF0000) >> 16);
	bytes.push_back((d & 0xFF000000) >> 24);
	return bytes;
}

#endif