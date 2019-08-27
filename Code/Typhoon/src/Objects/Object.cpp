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

TyObject operator+(const TyObject& left, const TyObject& right)
{
	return Ty_NULL;
}
