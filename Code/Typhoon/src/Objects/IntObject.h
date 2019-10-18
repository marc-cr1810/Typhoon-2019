#ifndef Ty_INTOBJECT_H
#define TY_INTOBJECT_H

#include "Object.h"

struct TyInt : TyObject
{
	TyInt()
	{
		Type = ObjectType::OBJECT_INT;
	}
};

#endif