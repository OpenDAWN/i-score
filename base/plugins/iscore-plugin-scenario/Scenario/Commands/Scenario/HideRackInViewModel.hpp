#pragma once
#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

class ConstraintViewModel;
class ConstraintModel;
class RackModel;
#include <tests/helpers/ForwardDeclaration.hpp>
namespace Scenario
{
    namespace Command
    {
        /**
         * @brief The HideRackInViewModel class
         *
         * For a given constraint view model, hides the rack.
         * Can only be called if a rack was being displayed.
         */
        class HideRackInViewModel final : public iscore::SerializableCommand
        {
                ISCORE_COMMAND_DECL(ScenarioCommandFactoryName(), HideRackInViewModel, "HideRackInViewModel")
#include <tests/helpers/FriendDeclaration.hpp>
            public:
                HideRackInViewModel(Path<ConstraintViewModel>&& path);

                /**
                 * @brief HideRackInViewModel
                 * @param constraint A constraint view model.
                 *
                 * Note : this will search it and make a path from an object named "BaseConstraintModel"
                 * Hence this constructor has to be used in a Scenario.
                 */
                HideRackInViewModel(const ConstraintViewModel& constraint);

                void undo() const override;
                void redo() const override;

            protected:
                void serializeImpl(DataStreamInput&) const override;
                void deserializeImpl(DataStreamOutput&) override;

            private:
                Path<ConstraintViewModel> m_constraintViewPath;

                Id<RackModel> m_constraintPreviousId {};
        };
    }
}
