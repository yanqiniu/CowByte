#include "Component.h"
#include "MessageBus.h"
#include "../Utils/CBQueue.h"
#include "../Utils/CBDebug.h"

Component::Component() :
    m_MessageBus(nullptr)
{

}

Component::~Component()
{

}

void Component::SetMessageBus(MessageBus *mBus)
{
    if (mBus == nullptr)
    {
        DbgWARNING("Trying to subscribe to null message bus!");
        return;
    }
    m_MessageBus = mBus;
}

void Component::_AcceptMessage(const Message &msg)
{
    m_MessageQueue.Enqueue(msg);
}

void Component::_HandleMessage(const Message &msg)
{
    if (msg.type == Message::DEFAULT)
    {
        DbgINFO("Message received!");
    }
}

bool Component::Initialize()
{
    return true;
}

bool Component::Update(const GameContext &context)
{
    return true;
}

bool Component::Shutdown()
{
    m_MessageBus = nullptr;
    m_MessageQueue.~CBQueue();
    m_Components.~CBVector();
    return true;
}

