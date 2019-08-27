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

	friend TyObject operator+(const TyObject& left, const TyObject& right);
};

extern TyObject _TyObject_Null;

#define Ty_NULL		_TyObject_Null;

#endif