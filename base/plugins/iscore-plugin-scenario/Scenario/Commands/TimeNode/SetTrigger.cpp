#include "SetTrigger.hpp"

#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Document/TimeNode/Trigger/TriggerModel.hpp>

#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include "iscore/document/DocumentInterface.hpp"

using namespace iscore;
using namespace Scenario::Command;


SetTrigger::SetTrigger(Path<TimeNodeModel>&& timeNodePath,
                       Trigger trigger) :
m_path {std::move(timeNodePath) },
m_trigger(std::move(trigger))
{
    auto& tn = m_path.find();
    m_previousTrigger = tn.trigger()->expression();
}

SetTrigger::~SetTrigger()
{

}

void SetTrigger::undo() const
{
    auto& tn = m_path.find();
    tn.trigger()->setExpression(m_previousTrigger);
}

void SetTrigger::redo() const
{
    auto& tn = m_path.find();
    tn.trigger()->setExpression(m_trigger);
}

void SetTrigger::serializeImpl(DataStreamInput& s) const
{
    s << m_path << m_trigger << m_previousTrigger;
}

void SetTrigger::deserializeImpl(DataStreamOutput& s)
{
    s >> m_path >> m_trigger >> m_previousTrigger;
}
