#include "ScenarioCreation_FromEvent.hpp"
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Process/Temporal/StateMachines/ScenarioStateMachine.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <Scenario/Commands/Scenario/Displacement/MoveNewEvent.hpp>
#include <Scenario/Commands/Scenario/Displacement/MoveNewState.hpp>
#include <Scenario/Commands/Scenario/Creations/CreateState.hpp>
#include <Scenario/Commands/Scenario/Creations/CreateEvent_State.hpp>


#include <Scenario/Commands/TimeNode/MergeTimeNodes.hpp>

#include <Scenario/Process/Temporal/StateMachines/Transitions/NothingTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/AnythingTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/StateTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/EventTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/ConstraintTransitions.hpp>
#include <Scenario/Process/Temporal/StateMachines/Transitions/TimeNodeTransitions.hpp>

#include <QFinalState>

using namespace Scenario::Command;
namespace Scenario
{
Creation_FromEvent::Creation_FromEvent(
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
    mainState->setObjectName("Main state");
    {
        auto pressed = new QState{mainState};
        auto released = new QState{mainState};
        auto move_nothing = new StrongQState<ScenarioElement::Nothing + Modifier::Move_tag::value>{mainState};
        auto move_state = new StrongQState<ScenarioElement::State + Modifier::Move_tag::value>{mainState};
        auto move_event = new StrongQState<ScenarioElement::Event + Modifier::Move_tag::value>{mainState};
        auto move_timenode = new StrongQState<ScenarioElement::TimeNode + Modifier::Move_tag::value>{mainState};

        pressed->setObjectName("Pressed");
        released->setObjectName("Released");
        move_nothing->setObjectName("Move on Nothing");
        move_state->setObjectName("Move on State");
        move_event->setObjectName("Move on Event");
        move_timenode->setObjectName("Move on TimeNode");

        // General setup
        mainState->setInitialState(pressed);
        released->addTransition(finalState);

        // Release
        make_transition<ReleaseOnAnything_Transition>(mainState, released);

        // Pressed -> ...
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
                       [&] () { rollback(); createToState(); });

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
            // Create a simple state where we are

            createInitialState();
            //createToNothing();
        });

        QObject::connect(move_nothing, &QState::entered, [&] ()
        {
            if(createdConstraints.empty() || createdEvents.empty())
            {
                rollback();
                return;
            }

            m_dispatcher.submitCommand<MoveNewEvent>(
                        Path<ScenarioModel>{m_scenarioPath},
                        createdConstraints.last(),
                        createdEvents.last(),
                        currentPoint.date,
                        currentPoint.y,
                        stateMachine.editionSettings().sequence());
        });

        QObject::connect(move_timenode , &QState::entered, [&] ()
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

        QObject::connect(released, &QState::entered, [&] ()
        {
            this->makeSnapshot();
            m_dispatcher.commit<Scenario::Command::CreationMetaCommand>();
        });
    }

    QState* rollbackState = new QState{this};
    rollbackState->setObjectName("Rollback");
    make_transition<Cancel_Transition>(mainState, rollbackState);
    rollbackState->addTransition(finalState);
    QObject::connect(rollbackState, &QState::entered, [&] ()
    {
        rollback();
    });

    setInitialState(mainState);
}

void Creation_FromEvent::createInitialState()
{
    auto cmd = new CreateState{m_scenarioPath, clickedEvent, currentPoint.y};
    m_dispatcher.submitCommand(cmd);

    createdStates.append(cmd->createdState());
}


// Note : clickedEvent is set at startEvent if clicking in the background.
void Creation_FromEvent::createToNothing()
{
    createInitialState();
    createToNothing_base(createdStates.first());
}

void Creation_FromEvent::createToState()
{
    createInitialState();
    createToState_base(createdStates.first());
}

void Creation_FromEvent::createToEvent()
{
    if(hoveredEvent != clickedEvent)
    {
        createInitialState();
        createToEvent_base(createdStates.first());
    }
}

void Creation_FromEvent::createToTimeNode()
{
    createInitialState();
    createToTimeNode_base(createdStates.first());
}
}
