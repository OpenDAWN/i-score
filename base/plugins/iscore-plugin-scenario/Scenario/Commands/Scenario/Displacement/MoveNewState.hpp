#pragma once

#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/tools/ModelPath.hpp>


class StateModel;
class ScenarioModel;

/*
 * Used on creation mode, when mouse is pressed and is moving.
 * In this case, only vertical move is allowed (new state on an existing event)
 */
namespace Scenario
{
    namespace Command
    {

        class MoveNewState final : public iscore::SerializableCommand
        {
            ISCORE_COMMAND_DECL(ScenarioCommandFactoryName(), MoveNewState, "MoveNewState")
            public:
            MoveNewState(
                Path<ScenarioModel>&& scenarioPath,
                const Id<StateModel>& stateId,
                const double y);

              void undo() const override;
              void redo() const override;

              void update(
                      const Path<ScenarioModel>&,
                      const Id<StateModel>&,
                      const double y)
              {
                  m_y = y;
              }

              const Path<ScenarioModel>& path() const
              { return m_path; }

            protected:
              void serializeImpl(DataStreamInput&) const override;
              void deserializeImpl(DataStreamOutput&) override;

            private:
              Path<ScenarioModel> m_path;
              Id<StateModel> m_stateId;
              double m_y{}, m_oldy{};
        };
    }
}
