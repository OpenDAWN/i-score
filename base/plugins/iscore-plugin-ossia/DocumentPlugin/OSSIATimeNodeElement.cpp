#include "OSSIATimeNodeElement.hpp"

#include <API/Headers/Editor/TimeNode.h>
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Document/TimeNode/Trigger/TriggerModel.hpp>
#include "iscore2OSSIA.hpp"

OSSIATimeNodeElement::OSSIATimeNodeElement(
        std::shared_ptr<OSSIA::TimeNode> ossia_tn,
        const TimeNodeModel& element,
        const DeviceList& devlist,
        QObject* parent):
    QObject{parent},
    m_ossia_node{ossia_tn},
    m_iscore_node{element},
    m_deviceList{devlist}
{
    connect(m_iscore_node.trigger(), &TriggerModel::triggered,
            this, [&] () {
        try {
        m_ossia_node->trigger();
        }
        catch(...)
        {

        }
    });
}

std::shared_ptr<OSSIA::TimeNode> OSSIATimeNodeElement::OSSIATimeNode() const
{
    return m_ossia_node;
}

void OSSIATimeNodeElement::recreate()
{
    on_triggerChanged(m_iscore_node.trigger()->expression());
}

void OSSIATimeNodeElement::clear()
{
    m_ossia_node->setExpression(OSSIA::ExpressionTrue);
}

void OSSIATimeNodeElement::on_triggerChanged(const iscore::Trigger& c)
try
{
    auto expr = iscore::convert::expression(c, m_deviceList);

    m_ossia_node->setExpression(expr);
}
catch(std::exception& e)
{
    qDebug() << e.what();
    m_ossia_node->setExpression(OSSIA::Expression::create(true));
}
