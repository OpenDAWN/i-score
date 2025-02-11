#include "MoveStates.hpp"
#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Process/Temporal/StateMachines/ScenarioStateMachine.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <Scenario/Process/Temporal/TemporalScenarioPresenter.hpp>


#include <QFinalState>
#include <Scenario/Process/Temporal/StateMachines/Transitions/AnythingTransitions.hpp>

// TODO a nice refactor is doable here between the three classes.
namespace Scenario
{
MoveConstraintState::MoveConstraintState(const ToolPalette& stateMachine,
                                         const Path<ScenarioModel>& scenarioPath,
                                         iscore::CommandStack& stack,
                                         iscore::ObjectLocker& locker,
                                         QState* parent):
    StateBase{scenarioPath, parent},
    m_dispatcher{stack}
{
    this->setObjectName("MoveConstraintState");
    using namespace Scenario::Command ;
    auto finalState = new QFinalState{this};

    QState* mainState = new QState{this};
    {
        QState* pressed = new QState{mainState};
        QState* released = new QState{mainState};
        QState* moving = new QState{mainState};

        // General setup
        mainState->setInitialState(pressed);
        released->addTransition(finalState);

        auto t_pressed =
                make_transition<MoveOnAnything_Transition>(
                    pressed, moving , *this);
        connect(t_pressed, &QAbstractTransition::triggered, [&] ()
        {
            auto& scenar = m_scenarioPath.find();
            m_constraintInitialStartDate= scenar.constraints.at(clickedConstraint).startDate();
            m_constraintInitialClickDate = currentPoint.date;
        });

        make_transition<ReleaseOnAnything_Transition>(
                    pressed, finalState);
        make_transition<MoveOnAnything_Transition>(
                    moving , moving , *this);
        make_transition<ReleaseOnAnything_Transition>(
                    moving , released);

        QObject::connect(moving, &QState::entered, [&] ()
        {
            m_dispatcher.submitCommand(
                            Path<ScenarioModel>{m_scenarioPath},
                            clickedConstraint,
                            m_constraintInitialStartDate + (currentPoint.date - m_constraintInitialClickDate),
                            currentPoint.y);
        });

        QObject::connect(released, &QState::entered, [&] ()
        {
            m_dispatcher.commit();
        });
    }

    QState* rollbackState = new QState{this};
    make_transition<Cancel_Transition>(mainState, rollbackState);
    rollbackState->addTransition(finalState);
    QObject::connect(rollbackState, &QState::entered, [&] ()
    {
        m_dispatcher.rollback();
    });

    setInitialState(mainState);
}


MoveEventState::MoveEventState(const ToolPalette& stateMachine,
                               const Path<ScenarioModel>& scenarioPath,
                               iscore::CommandStack& stack,
                               iscore::ObjectLocker& locker,
                               QState* parent):
    StateBase{scenarioPath, parent},
    m_dispatcher{stack}
{
    this->setObjectName("MoveEventState");
    using namespace Scenario::Command ;
    auto finalState = new QFinalState{this};

    QState* mainState = new QState{this};
    {
        QState* pressed = new QState{mainState};
        QState* released = new QState{mainState};
        QState* moving = new QState{mainState};

        // General setup
        mainState->setInitialState(pressed);
        released->addTransition(finalState);

        make_transition<MoveOnAnything_Transition>(
                    pressed, moving, *this);
        make_transition<ReleaseOnAnything_Transition>(
                    pressed, finalState);
        make_transition<MoveOnAnything_Transition>(
                    moving, moving, *this);
        make_transition<ReleaseOnAnything_Transition>(
                    moving, released);

        // What happens in each state.
        QObject::connect(moving, &QState::entered, [&] ()
        {
            Id<EventModel> evId{clickedEvent};
            if(!bool(evId) && bool(clickedState))
            {
                auto& scenar = m_scenarioPath.find();
                evId = scenar.state(clickedState).eventId();
            }

            m_dispatcher.submitCommand(
                            Path<ScenarioModel>{m_scenarioPath},
                            evId,
                            currentPoint.date,
                            stateMachine.editionSettings().expandMode());
        });

        QObject::connect(released, &QState::entered, [&] ()
        {
            m_dispatcher.commit();
        });
    }

    QState* rollbackState = new QState{this};
    make_transition<Cancel_Transition>(mainState, rollbackState);
    rollbackState->addTransition(finalState);
    QObject::connect(rollbackState, &QState::entered, [&] ()
    {
        m_dispatcher.rollback();
    });

    setInitialState(mainState);
}


MoveTimeNodeState::MoveTimeNodeState(const ToolPalette &stateMachine,
                                     const Path<ScenarioModel>& scenarioPath,
                                     iscore::CommandStack& stack,
                                     iscore::ObjectLocker& locker,
                                     QState* parent):
    StateBase{scenarioPath, parent},
    m_dispatcher{stack}
{
    this->setObjectName("MoveTimeNodeState");
    using namespace Scenario::Command ;
    auto finalState = new QFinalState{this};

    QState* mainState = new QState{this};
    {
        QState* pressed = new QState{mainState};
        QState* released = new QState{mainState};
        QState* moving = new QState{mainState};
        mainState->setInitialState(pressed);

        // General setup
        released->addTransition(finalState);

        make_transition<MoveOnAnything_Transition>(
                    pressed, moving, *this);
        make_transition<ReleaseOnAnything_Transition>(
                    pressed, finalState);
        make_transition<MoveOnAnything_Transition>(
                    moving, moving, *this);
        make_transition<ReleaseOnAnything_Transition>(
                    moving, released);

        // What happens in each state.
        QObject::connect(moving, &QState::entered, [&] ()
        {
            // Get the 1st event on the timenode.
            auto& scenar = m_scenarioPath.find();
            auto& tn = scenar.timeNodes.at(clickedTimeNode);
            const auto& ev_id = tn.events().first();
            auto date = currentPoint.date;

            if (!stateMachine.editionSettings().sequence())
                date = tn.date();

            m_dispatcher.submitCommand(
                            Path<ScenarioModel>{m_scenarioPath},
                            ev_id,
                            date,
                            stateMachine.editionSettings().expandMode());
        });

        QObject::connect(released, &QState::entered, [&] ()
        {
            m_dispatcher.commit();
        });
    }

    QState* rollbackState = new QState{this};
    make_transition<Cancel_Transition>(mainState, rollbackState);
    rollbackState->addTransition(finalState);
    QObject::connect(rollbackState, &QState::entered, [&] ()
    {
        m_dispatcher.rollback();
    });

    setInitialState(mainState);
}
}
