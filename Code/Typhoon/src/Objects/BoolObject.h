#ifndef Ty_BOOLOBJECT_H
#define TY_BOOLOBJECT_H

#include "Object.h"

struct TyBool : TyObject
{
	TyBool()
	{
		Type = ObjectType::OBJECT_BOOL;
	}
};

#endif