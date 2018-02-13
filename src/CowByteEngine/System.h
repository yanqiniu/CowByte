#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "UObject.h"

enum SystemType
{
	SYS_WINDOW,
	SYS_GAME,
	SYS_INPUT,
	SYS_GRAPHICS,
	SYS_GAMETIMER
};

struct SystemData
{

};

class System : public UObject
{
public:
	System();
	virtual ~System();

	SystemType GetType() const { return m_SystemType; }

protected:
	SystemType m_SystemType;
};

#endif