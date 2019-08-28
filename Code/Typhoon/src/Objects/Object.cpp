#include "Object.h"

#include "IntObject.h"
#include "UIntObject.h"
#include "FloatObject.h"
#include "DoubleObject.h"
#include "StringObject.h"
#include "BoolObject.h"

struct TyObject _TyObject_Null = {
	ObjectType::OBJECT_NULL
};

void TyObject::Set(Ty_int32_t value)
{
	Type = ObjectType::OBJECT_INT;
	ValueInt = value;
	ValueUInt = value;
	ValueFloat = value;
	ValueDouble = value;
	ValueString = std::to_string(value);
	ValueBool = value != 0;
}

void TyObject::Set(Ty_uint32_t value)
{
	Type = ObjectType::OBJECT_UINT;
	ValueInt = value;
	ValueUInt = value;
	ValueFloat = value;
	ValueDouble = value;
	ValueString = std::to_string(value);
	ValueBool = value != 0;
}

void TyObject::Set(Ty_float_t value)
{
	Type = ObjectType::OBJECT_FLOAT;
	ValueInt = (Ty_int32_t)value;
	ValueUInt = (Ty_uint32_t)value;
	ValueFloat = value;
	ValueDouble = value;
	ValueString = std::to_string(value);
	ValueBool = value != 0;
}

void TyObject::Set(Ty_double_t value)
{
	Type = ObjectType::OBJECT_DOUBLE;
	ValueInt = (Ty_int32_t)value;
	ValueUInt = (Ty_uint32_t)value;
	ValueFloat = value;
	ValueDouble = value;
	ValueString = std::to_string(value);
	ValueBool = value != 0;
}

void TyObject::Set(Ty_string_t value)
{
	Type = ObjectType::OBJECT_STRING;
	ValueInt = 0;
	ValueUInt = 0;
	ValueFloat = 0;
	ValueDouble = 0;
	ValueString = value;
	ValueBool = value != "";
}

void TyObject::Set(bool value)
{
	Type = ObjectType::OBJECT_BOOL;
	ValueInt = (Ty_int32_t)value;
	ValueUInt = (Ty_uint32_t)value;
	ValueFloat = (Ty_float_t)value;
	ValueDouble = (Ty_double_t)value;
	ValueString = value ? "True" : "False";
	ValueBool = value;
}

TyObject TyObject::operator=(const Ty_int32_t& other)
{
	TyObject object;
	object.Set(other);
	return object;
}

TyObject TyObject::operator=(const Ty_uint32_t& other)
{
	TyObject object;
	object.Set(other);
	return object;
}

TyObject TyObject::operator=(const Ty_float_t& other)
{
	TyObject object;
	object.Set(other);
	return object;
}

TyObject TyObject::operator=(const Ty_double_t& other)
{
	TyObject object;
	object.Set(other);
	return object;
}

TyObject TyObject::operator=(const Ty_string_t& other)
{
	TyObject object;
	object.Set(other);
	return object;
}

TyObject TyObject::operator=(const bool& other)
{
	TyObject object;
	object.Set(other);
	return object;
}

TyObject operator+(const TyObject& left, const TyObject& right)
{
	TyObject object;
	switch (left.Type)
	{
	case OBJECT_INT:
		object.Set(left.ValueInt + right.ValueInt);
		break;
	case OBJECT_UINT:
		object.Set(left.ValueUInt + right.ValueUInt);
		break;
	case OBJECT_FLOAT:
		object.Set(left.ValueFloat + right.ValueFloat);
		break;
	case OBJECT_DOUBLE:
		object.Set(left.ValueDouble + right.ValueDouble);
		break;
	case OBJECT_STRING:
		object.Set(left.ValueString + right.ValueString);
		break;
	case OBJECT_BOOL:
		object.Set((left.ValueInt + right.ValueInt) != 0);
		break;
	default:
		std::cout << "Cannot add to a NULL variable!" << std::endl;
		return Ty_NULL;
		break;
	}
	return object;
}

TyObject operator-(const TyObject& left, const TyObject& right)
{
	TyObject object;
	switch (left.Type)
	{
	case OBJECT_INT:
		object.Set(left.ValueInt - right.ValueInt);
		break;
	case OBJECT_UINT:
		object.Set(left.ValueUInt - right.ValueUInt);
		break;
	case OBJECT_FLOAT:
		object.Set(left.ValueFloat - right.ValueFloat);
		break;
	case OBJECT_DOUBLE:
		object.Set(left.ValueDouble - right.ValueDouble);
		break;
	case OBJECT_STRING:
		std::cout << "Cannot subtract a string from a string!" << std::endl;
		return Ty_NULL;
		break;
	case OBJECT_BOOL:
		object.Set((left.ValueInt - right.ValueInt) != 0);
		break;
	default:
		std::cout << "Cannot subtract a NULL variable!" << std::endl;
		return Ty_NULL;
		break;
	}
	return object;
}

TyObject operator*(const TyObject& left, const TyObject& right)
{
	TyObject object;
	switch (left.Type)
	{
	case OBJECT_INT:
		object.Set(left.ValueInt * right.ValueInt);
		break;
	case OBJECT_UINT:
		object.Set(left.ValueUInt * right.ValueUInt);
		break;
	case OBJECT_FLOAT:
		object.Set(left.ValueFloat * right.ValueFloat);
		break;
	case OBJECT_DOUBLE:
		object.Set(left.ValueDouble * right.ValueDouble);
		break;
	case OBJECT_STRING:
		std::cout << "Cannot multiply a string and a string!" << std::endl;
		return Ty_NULL;
		break;
	case OBJECT_BOOL:
		object.Set((left.ValueInt * right.ValueInt) != 0);
		break;
	default:
		std::cout << "Cannot multiply a NULL variable!" << std::endl;
		return Ty_NULL;
		break;
	}
	return object;
}

TyObject operator/(const TyObject& left, const TyObject& right)
{
	TyObject object;
	switch (left.Type)
	{
	case OBJECT_INT:
		object.Set(left.ValueInt / right.ValueInt);
		break;
	case OBJECT_UINT:
		object.Set(left.ValueUInt / right.ValueUInt);
		break;
	case OBJECT_FLOAT:
		object.Set(left.ValueFloat / right.ValueFloat);
		break;
	case OBJECT_DOUBLE:
		object.Set(left.ValueDouble / right.ValueDouble);
		break;
	case OBJECT_STRING:
		std::cout << "Cannot divide a string and a string!" << std::endl;
		return Ty_NULL;
		break;
	case OBJECT_BOOL:
		object.Set((left.ValueInt / right.ValueInt) != 0);
		break;
	default:
		std::cout << "Cannot divide a NULL variable!" << std::endl;
		return Ty_NULL;
		break;
	}
	return object;
}

TyObject operator%(const TyObject& left, const TyObject& right)
{
	TyObject object;
	switch (left.Type)
	{
	case OBJECT_INT:
		object.Set(left.ValueInt % right.ValueInt);
		break;
	case OBJECT_UINT:
		object.Set(left.ValueUInt % right.ValueUInt);
		break;
	case OBJECT_FLOAT:
		std::cout << "Cannot modulo a float and a float!" << std::endl;
		break;
	case OBJECT_DOUBLE:
		std::cout << "Cannot modulo a double and a double!" << std::endl;
		break;
	case OBJECT_STRING:
		std::cout << "Cannot modulo a string and a string!" << std::endl;
		return Ty_NULL;
		break;
	case OBJECT_BOOL:
		object.Set((left.ValueInt % right.ValueInt) != 0);
		break;
	default:
		std::cout << "Cannot divide a NULL variable!" << std::endl;
		return Ty_NULL;
		break;
	}
	return object;
}

bool operator==(const TyObject& left, const TyObject& right)
{
	switch (left.Type)
	{
	case OBJECT_NULL:
		left.Type == right.Type;
		break;
	case OBJECT_INT:
		left.ValueInt == right.ValueInt;
		break;
	case OBJECT_UINT:
		left.ValueUInt == right.ValueUInt;
		break;
	case OBJECT_FLOAT:
		left.ValueFloat == right.ValueFloat;
		break;
	case OBJECT_DOUBLE:
		left.ValueDouble == right.ValueDouble;
		break;
	case OBJECT_STRING:
		left.ValueString == right.ValueString;
		break;
	case OBJECT_BOOL:
		left.ValueBool == right.ValueBool;
		break;
	default:
		return false;
		break;
	}
}

bool operator!=(const TyObject& left, const TyObject& right)
{
	return !(left == right);
}

bool operator>(const TyObject& left, const TyObject& right)
{
	switch (left.Type)
	{
	case OBJECT_NULL:
		left.Type > right.Type;
		break;
	case OBJECT_INT:
		left.ValueInt > right.ValueInt;
		break;
	case OBJECT_UINT:
		left.ValueUInt > right.ValueUInt;
		break;
	case OBJECT_FLOAT:
		left.ValueFloat > right.ValueFloat;
		break;
	case OBJECT_DOUBLE:
		left.ValueDouble > right.ValueDouble;
		break;
	case OBJECT_STRING:
		left.ValueString.size() > right.ValueString.size();
		break;
	case OBJECT_BOOL:
		left.ValueBool > right.ValueBool;
		break;
	default:
		return false;
		break;
	}
}

bool operator<(const TyObject& left, const TyObject& right)
{
	switch (left.Type)
	{
	case OBJECT_NULL:
		left.Type < right.Type;
		break;
	case OBJECT_INT:
		left.ValueInt < right.ValueInt;
		break;
	case OBJECT_UINT:
		left.ValueUInt < right.ValueUInt;
		break;
	case OBJECT_FLOAT:
		left.ValueFloat < right.ValueFloat;
		break;
	case OBJECT_DOUBLE:
		left.ValueDouble < right.ValueDouble;
		break;
	case OBJECT_STRING:
		left.ValueString.size() < right.ValueString.size();
		break;
	case OBJECT_BOOL:
		left.ValueBool < right.ValueBool;
		break;
	default:
		return false;
		break;
	}
}

bool operator>=(const TyObject& left, const TyObject& right)
{
	return (left > right) || (left == right);
}

bool operator<=(const TyObject& left, const TyObject& right)
{
	return (left < right) || (left == right);
}
