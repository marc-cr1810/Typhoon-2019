#ifndef Ty_OBJECT_H
#define Ty_OBJECT_H

#include "../Port.h"

enum ObjectType
{
	OBJECT_NULL,
	OBJECT_INT,
	OBJECT_UINT,
	OBJECT_FLOAT,
	OBJECT_DOUBLE,
	OBJECT_STRING,
	OBJECT_BOOL
};

struct TyObject
{
	ObjectType Type;

	Ty_int32_t ValueInt = 0;
	Ty_uint32_t ValueUInt = 0;
	Ty_float_t ValueFloat = 0.0f;
	Ty_double_t ValueDouble = 0.0;
	Ty_string_t ValueString = "";
	bool ValueBool = false;

	void Set(Ty_int32_t value);
	void Set(Ty_uint32_t value);
	void Set(Ty_float_t value);
	void Set(Ty_double_t value);
	void Set(Ty_string_t value);
	void Set(bool value);

	TyObject operator=(const Ty_int32_t& other);
	TyObject operator=(const Ty_uint32_t& other);
	TyObject operator=(const Ty_float_t& other);
	TyObject operator=(const Ty_double_t& other);
	TyObject operator=(const Ty_string_t& other);
	TyObject operator=(const bool& other);

	friend TyObject operator+(const TyObject& left, const TyObject& right);
	friend TyObject operator-(const TyObject& left, const TyObject& right);
	friend TyObject operator*(const TyObject& left, const TyObject& right);
	friend TyObject operator/(const TyObject& left, const TyObject& right);
	friend TyObject operator%(const TyObject& left, const TyObject& right);

	friend bool operator==(const TyObject& left, const TyObject& right);
	friend bool operator!=(const TyObject& left, const TyObject& right);
	friend bool operator>(const TyObject& left, const TyObject& right);
	friend bool operator<(const TyObject& left, const TyObject& right);
	friend bool operator>=(const TyObject& left, const TyObject& right);
	friend bool operator<=(const TyObject& left, const TyObject& right);

	static void MultiplyString(const TyObject& left, const TyObject& right, TyObject* out);
};

extern TyObject _TyObject_Null;

#define Ty_NULL		_TyObject_Null;

#endif