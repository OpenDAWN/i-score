#include "BaseScenario.hpp"
#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Document/TimeNode/Trigger/TriggerModel.hpp>
#include <Scenario/Document/State/StateModel.hpp>
#include "Scenario/Document/Constraint/ConstraintModel.hpp"
#include <iscore/document/DocumentInterface.hpp>

class ConstraintViewModel;
BaseScenario::BaseScenario(const Id<BaseScenario>& id, QObject* parent):
    IdentifiedObject<BaseScenario>{id, "BaseScenario", parent},
    pluginModelList{iscore::IDocument::documentFromObject(parent), this},

    m_startNode{new TimeNodeModel{Id<TimeNodeModel>{0}, {0.2, 0.8}, TimeValue::zero(),  this}},
    m_endNode  {new TimeNodeModel{Id<TimeNodeModel>{1}, {0.2, 0.8}, TimeValue::zero(), this}},

    m_startEvent{new EventModel{Id<EventModel>{0}, m_startNode->id(), {0.4, 0.6}, TimeValue::zero(), this}},
    m_endEvent  {new EventModel{Id<EventModel>{1}, m_endNode->id(),   {0.4, 0.6}, TimeValue::zero(), this}},

    m_startState{new StateModel{Id<StateModel>{0}, m_startEvent->id(), 0, this}},
    m_endState  {new StateModel{Id<StateModel>{1}, m_endEvent->id(),   0, this}},

    m_constraint {new ConstraintModel{
                            Id<ConstraintModel>{0},
                            Id<ConstraintViewModel>{0},
                            0,
                            this}}
{
    m_startNode->addEvent(m_startEvent->id());
    m_endNode->addEvent(m_endEvent->id());
    m_endNode->trigger()->setActive(true);

    m_startEvent->addState(m_startState->id());
    m_endEvent->addState(m_endState->id());

    m_constraint->setStartState(m_startState->id());
    m_constraint->setEndState(m_endState->id());

    m_startState->setNextConstraint(m_constraint->id());
    m_endState->setPreviousConstraint(m_constraint->id());

    m_constraint->duration.setRigid(false);
    ConstraintDurations::Algorithms::changeAllDurations(*m_constraint, std::chrono::minutes{3});
    m_endEvent->setDate(m_constraint->duration.defaultDuration());
    m_endNode->setDate(m_constraint->duration.defaultDuration());
    m_constraint->setObjectName("BaseConstraintModel");
}

ConstraintModel* BaseScenario::findConstraint(const Id<ConstraintModel>& constraintId) const
{
    if(constraintId == m_constraint->id())
        return m_constraint;
    return nullptr;
}

EventModel* BaseScenario::findEvent(const Id<EventModel>& id) const
{
    if(id == m_startEvent->id())
    {
        return m_startEvent;
    }
    else if(id == m_endEvent->id())
    {
        return m_endEvent;
    }
    else
    {
        return nullptr;
    }
}

TimeNodeModel* BaseScenario::findTimeNode(const Id<TimeNodeModel>& id) const
{
    if(id == m_startNode->id())
    {
        return m_startNode;
    }
    else if(id == m_endNode->id())
    {
        return m_endNode;
    }
    else
    {
        return nullptr;
    }
}

StateModel* BaseScenario::findState(const Id<StateModel>& id) const
{
    if(id == m_startState->id())
    {
        return m_startState;
    }
    else if(id == m_endState->id())
    {
        return m_endState;
    }
    else
    {
        return nullptr;
    }
}


ConstraintModel& BaseScenario::baseConstraint() const
{
    return *m_constraint;
}


TimeNodeModel& BaseScenario::startTimeNode() const
{
    return *m_startNode;
}

TimeNodeModel& BaseScenario::endTimeNode() const
{
    return *m_endNode;
}


EventModel& BaseScenario::startEvent() const
{
    return *m_startEvent;
}

EventModel& BaseScenario::endEvent() const
{
    return *m_endEvent;
}


StateModel& BaseScenario::startState() const
{
    return *m_startState;
}

StateModel& BaseScenario::endState() const
{
    return *m_endState;
}


ConstraintModel &BaseScenario::constraint(const Id<ConstraintModel> &constraintId) const
{
    ISCORE_ASSERT(constraintId == m_constraint->id());
    return *m_constraint;
}

EventModel &BaseScenario::event(const Id<EventModel> &id) const
{
    ISCORE_ASSERT(id == m_startEvent->id() || id == m_endEvent->id());
    return id == m_startEvent->id()
            ? *m_startEvent
            : *m_endEvent;
}

TimeNodeModel &BaseScenario::timeNode(const Id<TimeNodeModel> &id) const
{
    ISCORE_ASSERT(id == m_startNode->id() || id == m_endNode->id());
    return id == m_startNode->id()
            ? *m_startNode
            : *m_endNode;
}

StateModel &BaseScenario::state(const Id<StateModel> &id) const
{
    ISCORE_ASSERT(id == m_startState->id() || id == m_endState->id());
    return id == m_startState->id()
            ? *m_startState
            : *m_endState;
}
