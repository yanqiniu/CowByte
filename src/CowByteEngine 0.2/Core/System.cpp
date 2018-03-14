#include "System.h"


SystemData::SystemData() :
    m_Type(SystemType::SYS_INVALID)
{

}

SystemData::SystemData(const SystemType type) :
    m_Type(type)
{

}

System::System(const SystemData& data) :
    Component(),
    m_SystemType(data.m_Type)
{
    m_CompType = Component::CompType::SYSTEM;
}

System::~System()
{
}
