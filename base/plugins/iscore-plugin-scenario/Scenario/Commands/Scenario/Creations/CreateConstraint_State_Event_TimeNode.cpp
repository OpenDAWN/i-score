#include "CreateConstraint_State_Event_TimeNode.hpp"

#include <Scenario/Process/Algorithms/StandardCreationPolicy.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <iscore/tools/SettableIdentifierGeneration.hpp>

#include <Scenario/Tools/RandomNameProvider.hpp>
using namespace Scenario::Command;
CreateConstraint_State_Event_TimeNode::CreateConstraint_State_Event_TimeNode(
        const ScenarioModel& scenario,
        const Id<StateModel>& startState,
        const TimeValue& date,
        double endStateY):
    m_newTimeNode{getStrongId(scenario.timeNodes)},
    m_createdName{RandomNameProvider::generateRandomName()},
    m_command{scenario,
              startState,
              m_newTimeNode,
              endStateY},
    m_date{date}
{

}

CreateConstraint_State_Event_TimeNode::CreateConstraint_State_Event_TimeNode(
        const Path<ScenarioModel>& scenarioPath,
        const Id<StateModel> &startState,
        const TimeValue &date,
        double endStateY):
    CreateConstraint_State_Event_TimeNode{scenarioPath.find(),
                                          startState, date, endStateY}
{

}

void CreateConstraint_State_Event_TimeNode::undo() const
{
    m_command.undo();

    ScenarioCreate<TimeNodeModel>::undo(
                m_newTimeNode,
                m_command.scenarioPath().find());
}

void CreateConstraint_State_Event_TimeNode::redo() const
{
    auto& scenar = m_command.scenarioPath().find();

    // Create the end timenode
    ScenarioCreate<TimeNodeModel>::redo(
                m_newTimeNode,
                {m_command.endStateY(), m_command.endStateY()},
                m_date,
                scenar);

    scenar.timeNode(m_newTimeNode).metadata.setName(m_createdName);

    // The event + state + constraint between
    m_command.redo();
}

void CreateConstraint_State_Event_TimeNode::serializeImpl(DataStreamInput& s) const
{
    s << m_newTimeNode
      << m_createdName
      << m_command.serialize()
      << m_date;
}

void CreateConstraint_State_Event_TimeNode::deserializeImpl(DataStreamOutput& s)
{
    QByteArray b;
    s >> m_newTimeNode
            >> m_createdName
            >> b
            >> m_date;

    m_command.deserialize(b);
}
