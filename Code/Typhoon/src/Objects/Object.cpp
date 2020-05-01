#include "Object.h"

#include "IntObject.h"
#include "UIntObject.h"
#include "FloatObject.h"
#include "DoubleObject.h"
#include "StringObject.h"
#include "BoolObject.h"

struct TyObject _TyObject_Null = {
	ObjectType::OBJECT_NULL, 0, 0, 0.0f, 0.0, "NULL", false
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

void TyObject::Add(TyObject& object)
{
	ValueList.push_back(object);

	ValueInt = ValueList.size();
	ValueUInt = ValueList.size();
	ValueFloat = ValueList.size();
	ValueDouble = ValueList.size();
	ValueString = (ValueString != "" ? ValueString.substr(0, ValueString.size() - 2) + ", " : "[ ") + object.ValueString + " ]";
	ValueBool = true;
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
	if (right.Type == ObjectType::OBJECT_STRING)
	{
		object.Set(left.ValueString + right.ValueString);
	}
	else
	{
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

	if (right.Type == ObjectType::OBJECT_STRING)
	{
		TyObject::MultiplyString(left, right, &object);
	}
	else
	{
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
				TyObject::MultiplyString(left, right, &object);
				break;
			case OBJECT_BOOL:
				object.Set((left.ValueInt * right.ValueInt) != 0);
				break;
			default:
				std::cout << "Cannot multiply a NULL variable!" << std::endl;
				return Ty_NULL;
				break;
		}
	}
	return object;
}

TyObject operator/(const TyObject& left, const TyObject& right)
{
	TyObject object;
	switch (left.Type)
	{
		case OBJECT_INT:
		{
			float value = left.ValueFloat / right.ValueFloat;
			if ((value - (int)value) > 0)
				object.Set(value);
			else
				object.Set(left.ValueInt / right.ValueInt);
		}
		break;
		case OBJECT_UINT:
		{
			float value = left.ValueFloat / right.ValueFloat;
			if ((value - (int)value) > 0)
				object.Set(value);
			else
				object.Set(left.ValueUInt / right.ValueUInt);
		}
		break;
		case OBJECT_FLOAT:
			object.Set(left.ValueFloat / right.ValueFloat);
			break;
		case OBJECT_DOUBLE:
			object.Set(left.ValueDouble / right.ValueDouble);
			break;
		case OBJECT_STRING:
			std::cout << "Cannot divide a string!" << std::endl;
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
			return left.Type == right.Type;
		case OBJECT_INT:
			return left.ValueInt == right.ValueInt;
		case OBJECT_UINT:
			return left.ValueUInt == right.ValueUInt;
		case OBJECT_FLOAT:
			return left.ValueFloat == right.ValueFloat;
		case OBJECT_DOUBLE:
			return left.ValueDouble == right.ValueDouble;
		case OBJECT_STRING:
			return left.ValueString == right.ValueString;
		case OBJECT_BOOL:
			return left.ValueBool == right.ValueBool;
		default:
			return false;
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
			return left.Type > right.Type;
		case OBJECT_INT:
			return left.ValueInt > right.ValueInt;
		case OBJECT_UINT:
			return left.ValueUInt > right.ValueUInt;
		case OBJECT_FLOAT:
			return left.ValueFloat > right.ValueFloat;
		case OBJECT_DOUBLE:
			return left.ValueDouble > right.ValueDouble;
		case OBJECT_STRING:
			return left.ValueString.size() > right.ValueString.size();
		case OBJECT_BOOL:
			return left.ValueBool > right.ValueBool;
		default:
			return false;
	}
}

bool operator<(const TyObject& left, const TyObject& right)
{
	switch (left.Type)
	{
		case OBJECT_NULL:
			return left.Type < right.Type;
		case OBJECT_INT:
			return left.ValueInt < right.ValueInt;
		case OBJECT_UINT:
			return left.ValueUInt < right.ValueUInt;
		case OBJECT_FLOAT:
			return left.ValueFloat < right.ValueFloat;
		case OBJECT_DOUBLE:
			return left.ValueDouble < right.ValueDouble;
		case OBJECT_STRING:
			return left.ValueString.size() < right.ValueString.size();
		case OBJECT_BOOL:
			return left.ValueBool < right.ValueBool;
		default:
			return false;
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

void TyObject::MultiplyString(const TyObject& left, const TyObject& right, TyObject* out)
{
	const TyObject* string = &left;
	const TyObject* count = &right;
	if (right.Type == ObjectType::OBJECT_STRING)
	{
		string = &right;
		count = &left;
	}

	if (count->Type == OBJECT_INT)
	{
		for (int i = 0; i < count->ValueInt; i++)
		{
			Ty_string_t value = out->ValueString + string->ValueString;
			out->Set(value);
		}
	}
	else if (count->Type == OBJECT_FLOAT)
	{
		for (int i = 0; i < (Ty_int32_t)count->ValueFloat; i++)
		{
			Ty_string_t value = out->ValueString + string->ValueString;
			out->Set(value);
		}

		float test = count->ValueFloat - (Ty_int32_t)count->ValueFloat;
		int size = string->ValueString.size() * (count->ValueFloat - (Ty_int32_t)count->ValueFloat);
		for (int i = 0; i < size; i++)
		{
			Ty_string_t value = out->ValueString + string->ValueString[i];
			out->Set(value);
		}
	}
	else if (count->Type == OBJECT_DOUBLE)
	{
		for (int i = 0; i < (Ty_int32_t)count->ValueDouble; i++)
		{
			Ty_string_t value = out->ValueString + string->ValueString;
			out->Set(value);
		}

		float test = count->ValueDouble - (Ty_int32_t)count->ValueDouble;
		int size = left.ValueString.size() * (count->ValueDouble - (Ty_int32_t)count->ValueDouble);
		for (int i = 0; i < size; i++)
		{
			Ty_string_t value = out->ValueString + string->ValueString[i];
			out->Set(value);
		}
	}
	else
	{
		std::cout << "Cannot multiply a string by a non-integer or non-floating-point type!" << std::endl;
		out = &Ty_NULL;
	}
}