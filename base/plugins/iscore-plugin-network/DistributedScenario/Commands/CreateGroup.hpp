#pragma once
#include <DistributedScenario/Commands/DistributedScenarioCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ObjectPath.hpp>
#include <iscore/tools/IdentifiedObject.hpp>

class Group;
class CreateGroup : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL(
                DistributedScenarioCommandFactoryName(),
                CreateGroup,
                "CreateGroup")
        public:
        CreateGroup(ObjectPath&& groupMgrPath, QString groupName);

        void undo() const override;
        void redo() const override;

        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        ObjectPath m_path;
        QString m_name;
        Id<Group> m_newGroupId;
};
