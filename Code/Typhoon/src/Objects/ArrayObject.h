#ifndef Ty_ARRAYOBJECT_H
#define TY_ARRAYOBJECT_H

#include "Object.h"

struct TyArray : TyObject
{
	TyArray()
	{
		Type = ObjectType::OBJECT_ARRAY;
	}
};

#endif