#ifndef Ty_FLOATOBJECT_H
#define TY_FLOATOBJECT_H

#include "Object.h"

struct TyFloat : TyObject
{
	TyFloat()
	{
		Type = ObjectType::OBJECT_FLOAT;
	}
};

#endif