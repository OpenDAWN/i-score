#pragma once
#include "NetworkDocumentPlugin.hpp"

#include <Repartition/session/MasterSession.hpp>
namespace iscore
{
    class CommandStack;
    class ObjectLocker;
}
class MasterNetworkPolicy : public NetworkPluginPolicy
{
    public:
        MasterNetworkPolicy(MasterSession* s,
                            iscore::DocumentContext& c);

        MasterSession* session() const override
        { return m_session; }

    private:
        MasterSession* m_session{};
};
