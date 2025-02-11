#pragma once
#include <iscore/plugins/documentdelegate/plugin/ElementPluginModel.hpp>

#include <State/Expression.hpp>
#include <memory>
class TimeNodeModel;
class DeviceList;
namespace OSSIA
{
    class TimeNode;
}

class OSSIATimeNodeElement final : public QObject
{
    public:
        OSSIATimeNodeElement(
                std::shared_ptr<OSSIA::TimeNode> ossia_tn,
                const TimeNodeModel& element,
                const DeviceList& devlist,
                QObject* parent);

        std::shared_ptr<OSSIA::TimeNode> OSSIATimeNode() const;

        void recreate();
        void clear();

    private:
        void on_triggerChanged(const iscore::Trigger& c);

        std::shared_ptr<OSSIA::TimeNode> m_ossia_node;
        const TimeNodeModel& m_iscore_node;

        const DeviceList& m_deviceList;
};
