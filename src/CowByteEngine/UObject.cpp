#include "UObject.h"

int UObject::m_ObjectAmount = 0;
int UObject::m_IDCounter = 0;

UObject::UObject(const std::tstring name) :
    m_Name(name),
    m_ID(m_IDCounter),

    m_bIsInitialized(false),
    m_bIsPostInitialized(false),
    m_bIsContentLoaded(false),
    m_bIsPostContentLoaded(false),
    m_bIsDestroyed(false),

    m_bCanDraw(true),
    m_bCanTick(true)
{
    ++m_ObjectAmount;
    ++m_IDCounter;
}

UObject::~UObject()
{
    --m_ObjectAmount;
}

void UObject::Reset()
{

}
