#ifndef Ty_DOUBLEOBJECT_H
#define TY_DOUBLEOBJECT_H

#include "Object.h"

struct TyDouble : TyObject
{
	TyDouble()
	{
		Type = ObjectType::OBJECT_DOUBLE;
	}
};

#endif