#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "UObject.h"
#include "context.h"

enum SystemType
{
    SYS_INVALID,
	SYS_WINDOW,
	SYS_GAME,
	SYS_INPUT,
	SYS_GRAPHICS,
	SYS_GAMETIMER,

    SYS_MAX
};

struct SystemData
{
    SystemData();
    SystemData(const SystemType type);
    SystemType m_Type;
};

class System : public UObject
{
    friend class Engine;
public:
	System(const SystemData& data);
	virtual ~System();

    virtual bool Initialize()                       { return UObject::Initialize(); }
    virtual bool Update(Context& context)           { return UObject::Update(context); }
    virtual bool ShutDown()                         { return UObject::ShutDown(); }

	SystemType GetType() const { return m_SystemType; }

protected:
	SystemType m_SystemType;
};

#endif