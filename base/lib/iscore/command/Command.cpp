#include "Command.hpp"
#include <core/application/Application.hpp>

iscore::Command::Command():
    context{iscore::Application::instance()}
{

}

iscore::Command::~Command()
{

}

quint32 iscore::Command::timestamp() const
{
    return static_cast<quint32>(m_timestamp.count());
}

void iscore::Command::setTimestamp(quint32 stmp)
{
    m_timestamp = std::chrono::duration<quint32> (stmp);
}
