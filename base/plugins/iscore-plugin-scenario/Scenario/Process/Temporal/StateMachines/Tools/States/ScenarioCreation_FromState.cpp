#include "ScenarioCreation_FromState.hpp"
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Process/Temporal/StateMachines/ScenarioStateMachine.hpp>
#include <Scenario/Process/ScenarioModel.hpp>


#include <Scenario/Commands/Scenario/Creations/CreateState.hpp>
#include <Scenario/Commands/Scenario/Displacement/MoveNewEvent.hpp>
#include <Scenario/Commands/Scenario/Displacement/MoveNewState.hpp>

#include <Scenario/Commands/Scenario/Creations/CreateConstraint.hpp>
#include <Scenario/Commands/TimeNode/MergeTimeNodes.hpp>

#include <Scenario/Process/Temporal/StateMachines/Transitions/NothingTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/AnythingTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/EventTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/ConstraintTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/TimeNodeTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/StateTransitions.hpp>

#include <Scenario/Process/Temporal/StateMachines/Tools/ScenarioRollbackStrategy.hpp>
#include <QFinalState>

using namespace Scenario::Command;

namespace Scenario
{
Creation_FromState::Creation_FromState(
        const ToolPalette& stateMachine,
        const Path<ScenarioModel>& scenarioPath,
        iscore::CommandStack& stack,
        QState* parent):
    CreationState{stateMachine, stack, std::move(scenarioPath), parent}
{
    using namespace Scenario::Command;
    auto finalState = new QFinalState{this};
    connect(finalState, &QState::entered, [&] ()
    {
        clearCreatedIds();
    });

    QState* mainState = new QState{this};
    {
        auto pressed = new QState{mainState};
        auto released = new QState{mainState};
        auto move_nothing = new StrongQState<ScenarioElement::Nothing + Modifier::Move_tag::value>{mainState};
        auto move_state = new StrongQState<ScenarioElement::State + Modifier::Move_tag::value>{mainState};
        auto move_event = new StrongQState<ScenarioElement::Event + Modifier::Move_tag::value>{mainState};
        auto move_timenode = new StrongQState<ScenarioElement::TimeNode + Modifier::Move_tag::value>{mainState};

        // General setup
        mainState->setInitialState(pressed);
        released->addTransition(finalState);

        // Release
        make_transition<ReleaseOnAnything_Transition>(mainState, released);

        // Pressed -> ...
        make_transition<MoveOnNothing_Transition>(pressed, move_state, *this);
        make_transition<MoveOnNothing_Transition>(pressed, move_nothing, *this);

        /// MoveOnNothing -> ...
        // MoveOnNothing -> MoveOnNothing.
        make_transition<MoveOnNothing_Transition>(move_nothing, move_nothing, *this);

        // MoveOnNothing -> MoveOnState.
        add_transition(move_nothing, move_state,
                       [&] () { rollback(); createToState(); });

        // MoveOnNothing -> MoveOnEvent.
        add_transition(move_nothing, move_event,
                       [&] () { rollback(); createToEvent(); });

        // MoveOnNothing -> MoveOnTimeNode
        add_transition(move_nothing, move_timenode,
                       [&] () { rollback(); createToTimeNode(); });


        /// MoveOnState -> ...
        // MoveOnState -> MoveOnNothing
        add_transition(move_state, move_nothing,
                       [&] () { rollback(); createToNothing(); });

        // MoveOnState -> MoveOnState
        // We don't do anything, the constraint should not move.

        // MoveOnState -> MoveOnEvent
        add_transition(move_state, move_event,
                       [&] () { rollback(); createToEvent(); });

        // MoveOnState -> MoveOnTimeNode
        add_transition(move_state, move_timenode,
                       [&] () { rollback(); createToTimeNode(); });

        /// MoveOnEvent -> ...
        // MoveOnEvent -> MoveOnNothing
        add_transition(move_event, move_nothing,
                       [&] () { rollback(); createToNothing(); });

        // MoveOnEvent -> MoveOnState
        add_transition(move_event, move_state,
                       [&] () {
            if(m_parentSM.model().state(clickedState).eventId() != m_parentSM.model().state(hoveredState).eventId())
            {
                rollback();
                createToState();
            }
        });

        // MoveOnEvent -> MoveOnEvent
        make_transition<MoveOnEvent_Transition>(move_event, move_event, *this);

        // MoveOnEvent -> MoveOnTimeNode
        add_transition(move_event, move_timenode,
                       [&] () { rollback(); createToTimeNode(); });


        /// MoveOnTimeNode -> ...
        // MoveOnTimeNode -> MoveOnNothing
        add_transition(move_timenode, move_nothing,
                       [&] () { rollback(); createToNothing(); });

        // MoveOnTimeNode -> MoveOnState
        add_transition(move_timenode, move_state,
                       [&] () { rollback(); createToState(); });

        // MoveOnTimeNode -> MoveOnEvent
        add_transition(move_timenode, move_event,
                       [&] () { rollback(); createToEvent(); });

        // MoveOnTimeNode -> MoveOnTimeNode
        make_transition<MoveOnTimeNode_Transition>(move_timenode , move_timenode , *this);



        // What happens in each state.
        QObject::connect(pressed, &QState::entered,
                         [&] ()
        {
            m_clickedPoint = currentPoint;
            createToNothing();
        });

        QObject::connect(move_nothing, &QState::entered, [&] ()
        {
            if(createdConstraints.empty() || createdEvents.empty())
            {
                rollback();
                return;
            }

            if(m_parentSM.editionSettings().sequence())
            {
                const auto&  st = m_parentSM.model().state(clickedState);
                currentPoint.y = st.heightPercentage();
            }

            m_dispatcher.submitCommand<MoveNewEvent>(
                        Path<ScenarioModel>{m_scenarioPath},
                        createdConstraints.last(),
                        createdEvents.last(),
                        currentPoint.date,
                        currentPoint.y,
                        stateMachine.editionSettings().sequence());

        });

        QObject::connect(move_event, &QState::entered, [&] ()
        {
            if(createdStates.empty())
            {
                rollback();
                return;
            }

            m_dispatcher.submitCommand<MoveNewState>(
                        Path<ScenarioModel>{m_scenarioPath},
                        createdStates.last(),
                        currentPoint.y);
        });

        QObject::connect(move_timenode, &QState::entered, [&] ()
        {
            if(createdStates.empty())
            {
                rollback();
                return;
            }

            m_dispatcher.submitCommand<MoveNewState>(
                        Path<ScenarioModel>{m_scenarioPath},
                        createdStates.last(),
                        currentPoint.y);
        });

        QObject::connect(released, &QState::entered, [&] ()
        {
            this->makeSnapshot();
            m_dispatcher.commit<Scenario::Command::CreationMetaCommand>();
        });
    }

    QState* rollbackState = new QState{this};
    make_transition<Cancel_Transition>(mainState, rollbackState);
    rollbackState->addTransition(finalState);
    QObject::connect(rollbackState, &QState::entered, [&] ()
    {
        rollback();
    });

    setInitialState(mainState);
}


template<typename Fun>
void Creation_FromState::creationCheck(Fun&& fun)
{
    const auto& scenar = m_parentSM.model();
    if(!m_parentSM.editionSettings().sequence())
    {
        // Create new state at the beginning
        auto cmd = new Scenario::Command::CreateState{m_scenarioPath, scenar.state(clickedState).eventId(), currentPoint.y};
        m_dispatcher.submitCommand(cmd);

        createdStates.append(cmd->createdState());
        fun(createdStates.first());
    }
    else
    {
        const auto& st = scenar.states.at(clickedState);
        if(!st.nextConstraint()) // TODO & deltaY < deltaX
        {
            currentPoint.y = st.heightPercentage();
            fun(clickedState);
        }
        else
        {
            ISCORE_TODO;
            // create a single state on the same event (deltaY > deltaX)
        }
    }
}

// Note : clickedEvent is set at startEvent if clicking in the background.
void Creation_FromState::createToNothing()
{
    creationCheck([&] (const Id<StateModel>& id) { createToNothing_base(id); });
}

void Creation_FromState::createToTimeNode()
{
    creationCheck([&] (const Id<StateModel>& id) { createToTimeNode_base(id); });
}

void Creation_FromState::createToEvent()
{
    if(hoveredEvent == m_parentSM.model().state(clickedState).eventId())
    {
        creationCheck([&] (const Id<StateModel>& id) { });
    }
    else
    {
        creationCheck([&] (const Id<StateModel>& id) { createToEvent_base(id); });
    }
}

void Creation_FromState::createToState()
{
    if(!m_parentSM.model().states.at(hoveredState).previousConstraint())
    {
        // No previous constraint -> we create a new constraint and link it to this state
        creationCheck([&] (const Id<StateModel>& id) { createToState_base(id); });
    }
    else
    {
        // Previous constraint -> we add a new state to the event and link to it.
        this->hoveredEvent = m_parentSM.model().states.at(hoveredState).eventId();
        creationCheck([&] (const Id<StateModel>& id) { createToEvent_base(id); });
    }
}
}
