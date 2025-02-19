#pragma once
#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

#include <tests/helpers/ForwardDeclaration.hpp>

class RackModel;
namespace Scenario
{
    namespace Command
    {
        class DuplicateRack final : public iscore::SerializableCommand
        {
                ISCORE_COMMAND_DECL(ScenarioCommandFactoryName(), DuplicateRack, "DuplicateRack")
#include <tests/helpers/FriendDeclaration.hpp>
            public:
                DuplicateRack(ObjectPath&& rackToCopy);

                void undo() const override;
                void redo() const override;

            protected:
                void serializeImpl(DataStreamInput&) const override;
                void deserializeImpl(DataStreamOutput&) override;

            private:
                ObjectPath m_rackPath;

                Id<RackModel> m_newRackId;
        };
    }
}
