#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "Component.h"
#include "GameContext.h"

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

class System : public Component
{
    friend class Engine;

public: 
    SystemType GetType() { return m_SystemType; }

protected:
    System(const SystemData& data);
    virtual ~System();

    virtual bool Initialize() { return Component::Initialize(); }
    virtual bool Update(GameContext& context) { return Component::Update(context); }
    virtual bool Shutdown() { return Component::Shutdown(); }

    SystemType GetType() const { return m_SystemType; }

    SystemType m_SystemType;
};

#endif