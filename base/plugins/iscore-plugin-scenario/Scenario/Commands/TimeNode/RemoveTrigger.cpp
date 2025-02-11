#include "RemoveTrigger.hpp"

#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Document/TimeNode/Trigger/TriggerModel.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

using namespace Scenario::Command;

RemoveTrigger::RemoveTrigger(Path<TimeNodeModel>&& timeNodePath):
    m_path{std::move(timeNodePath)}
{

}

RemoveTrigger::~RemoveTrigger()
{
    qDeleteAll(m_cmds);
}

void RemoveTrigger::undo() const
{
    auto& tn = m_path.find();
    tn.trigger()->setActive(true);

    for (auto cmd : m_cmds)
    {
        cmd->undo();
        delete cmd;
    }
    m_cmds.clear();
}

void RemoveTrigger::redo() const
{
    auto& tn = m_path.find();
    tn.trigger()->setActive(false);

    ScenarioModel* scenar = safe_cast<ScenarioModel*>(tn.parentScenario());

    for (auto& cstrId : scenar->constraintsBeforeTimeNode(tn.id()))
    {
        auto cmd = new SetRigidity(scenar->constraints.at(cstrId), true);
        cmd->redo();
        m_cmds.push_back(cmd);
    }
}

void RemoveTrigger::serializeImpl(DataStreamInput& s) const
{
    s << m_path;
    s << m_cmds.count();

    for(const auto& cmd : m_cmds)
    {
        s << cmd->serialize();
    }
}

void RemoveTrigger::deserializeImpl(DataStreamOutput& s)
{
    int n;
    s >> m_path;
    s >> n;
        for(;n-->0;)
        {
            QByteArray a;
            s >> a;
            auto cmd = new SetRigidity;
            cmd->deserialize(a);
            m_cmds.append(cmd);
        }
}
