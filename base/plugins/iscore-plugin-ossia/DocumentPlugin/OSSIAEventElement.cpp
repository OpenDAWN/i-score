#include "OSSIAEventElement.hpp"
#include "iscore2OSSIA.hpp"

#include <Scenario/Document/Event/EventModel.hpp>
#include <API/Headers/Editor/TimeEvent.h>

OSSIAEventElement::OSSIAEventElement(
        std::shared_ptr<OSSIA::TimeEvent> event,
        const EventModel &element,
        const DeviceList& deviceList,
        QObject* parent):
    QObject{parent},
    m_iscore_event{element},
    m_ossia_event{event},
    m_deviceList{deviceList}
{
}

std::shared_ptr<OSSIA::TimeEvent> OSSIAEventElement::OSSIAEvent() const
{
    return m_ossia_event;
}

void OSSIAEventElement::recreate()
{
    on_conditionChanged(m_iscore_event.condition());
}

void OSSIAEventElement::clear()
{
    m_ossia_event->setExpression(OSSIA::ExpressionTrue);
}

void OSSIAEventElement::on_conditionChanged(const iscore::Condition& c)
try
{
    auto expr = iscore::convert::expression(c, m_deviceList);

    m_ossia_event->setExpression(expr);
}
catch(std::exception& e)
{
    qDebug() << e.what();
    m_ossia_event->setExpression(OSSIA::Expression::create(true));
}
