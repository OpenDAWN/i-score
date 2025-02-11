#pragma once
#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

#include <Process/ExpandMode.hpp>
#include <tests/helpers/ForwardDeclaration.hpp>
#include <Process/TimeValue.hpp>

class ConstraintModel;
class ScenarioModel;

/*
 * Command for vertical move so it does'nt have to resize anything on time axis
 * */

namespace Scenario
{
    namespace Command
    {
        class MoveConstraint final : public iscore::SerializableCommand
        {
                ISCORE_COMMAND_DECL(ScenarioCommandFactoryName(), MoveConstraint, "MoveConstraint")
#include <tests/helpers/FriendDeclaration.hpp>
            public:
                MoveConstraint(
                        Path<ScenarioModel>&& scenarioPath,
                    const Id<ConstraintModel>& id,
                    const TimeValue& date,
                    double y);

                void update(const Path<ScenarioModel>&,
                            const Id<ConstraintModel>& ,
                            const TimeValue& date,
                            double height);

                void undo() const override;
                void redo() const override;

            protected:
                void serializeImpl(DataStreamInput&) const override;
                void deserializeImpl(DataStreamOutput&) override;

            private:
                Path<ScenarioModel> m_path;
                Id<ConstraintModel> m_constraint;
                double m_oldHeight{},
                       m_newHeight{};
        };
    }
}
