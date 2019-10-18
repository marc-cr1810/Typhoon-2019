#ifndef Ty_STRINGOBJECT_H
#define TY_STRINGOBJECT_H

#include "Object.h"

struct TyString : TyObject
{
	TyString()
	{
		Type = ObjectType::OBJECT_STRING;
	}
};

#endif