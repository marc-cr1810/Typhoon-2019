#ifndef Ty_UINTOBJECT_H
#define TY_UINTOBJECT_H

#include "Object.h"

struct TyUInt : TyObject
{
	TyUInt()
	{
		Type = ObjectType::OBJECT_UINT;
	}
};

#endif