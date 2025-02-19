#include "UpdateAddressSettings.hpp"

#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <boost/range/algorithm/find_if.hpp>

using namespace iscore;
using namespace DeviceExplorer::Command;
UpdateAddressSettings::UpdateAddressSettings(
        Path<DeviceDocumentPlugin>&& device_tree,
        const iscore::NodePath& node,
        const iscore::AddressSettings& parameters):
    m_devicesModel{device_tree},
    m_node(node),
    m_newParameters(parameters)
{
    auto& devplug = m_devicesModel.find();
    auto n = m_node.toNode(&devplug.rootNode());
    ISCORE_ASSERT(n);

    m_oldParameters = n->get<iscore::AddressSettings>();
}

void UpdateAddressSettings::undo() const
{
    auto& devplug = m_devicesModel.find();
    devplug.updateProxy.updateAddress(m_node, m_oldParameters);
}

void UpdateAddressSettings::redo() const
{
    auto& devplug = m_devicesModel.find();
    devplug.updateProxy.updateAddress(m_node, m_newParameters);
}

void UpdateAddressSettings::serializeImpl(DataStreamInput& d) const
{
    d << m_devicesModel
      << m_oldParameters
      << m_newParameters;
}

void UpdateAddressSettings::deserializeImpl(DataStreamOutput& d)
{
    d >> m_devicesModel
      >> m_oldParameters
      >> m_newParameters;
}
