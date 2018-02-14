#ifndef _UOBJECT_H
#define _UOBJECT_H

#include "context.h"
#include "string.h"

class UObject
{
public:
    UObject();
    UObject(const std::tstring name);
    virtual ~UObject();

    //void* operator new(size_t size);
    //void operator delete(void* pDelete);

    virtual bool Initialize()                     { m_bIsInitialized = true; return true; }
    virtual bool PostInitialize()                 { m_bIsPostInitialized = true; return true; }
    virtual bool LoadContent()                    { m_bIsContentLoaded = true; return true; }
    virtual bool PostLoadContent()                { m_bIsPostContentLoaded = true; return true; }
    virtual bool Update(Context& context)         { return true; }
    //virtual bool LateUpdate(Context& context);  { return true; }
    virtual bool Draw(Context& context)           { return true; }
    virtual bool DrawUI(Context& context)         { return true; }
    virtual bool ShutDown()                       { return true; }

    virtual void Reset();

    void SetName(std::tstring& name) { m_Name = name; }
    const std::tstring& GetName() { return m_Name; }

    bool IsInitialized() const       { return m_bIsInitialized; }
    bool IsPostInitialized() const   { return m_bIsPostInitialized; }
    bool IsContentLoaded() const     { return m_bIsContentLoaded; }
    bool IsPostContentLoaded() const { return m_bIsPostContentLoaded; }

    void SetCanTick(bool canTick) { m_bCanTick = canTick; }
    bool CanTick() const { return m_bCanTick; }
    void SetCanDraw(bool canDraw) { m_bCanDraw = canDraw; }
    bool CanDraw() const { return CanDraw(); }

    void Destroy() { m_bIsDestroyed = true; }
    bool IsDestroyed() const { return m_bIsDestroyed; }

    void Activate() { m_bIsActivated = true; }
    void DeActivate() { m_bIsActivated = false; }
    bool IsActivated() const { return m_bIsActivated; }

protected:

    static int m_ObjectAmount;
    static unsigned int m_IDCounter;
    unsigned int m_ID;
    std::tstring m_Name;


    bool m_bIsInitialized : 1;
    bool m_bIsPostInitialized : 1;

    bool m_bIsContentLoaded : 1;
    bool m_bIsPostContentLoaded : 1;


private:
    bool m_bIsActivated : 1;
    bool m_bIsDestroyed : 1;

    bool m_bCanTick : 1;
    bool m_bCanDraw : 1;
};

#endif // ifndef _UOBJECT_H
