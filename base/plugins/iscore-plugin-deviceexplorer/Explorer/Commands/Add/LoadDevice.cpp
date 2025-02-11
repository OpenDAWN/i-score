#include "LoadDevice.hpp"
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>

LoadDevice::LoadDevice(
        Path<DeviceDocumentPlugin>&& device_tree,
        iscore::Node&& node):
    m_devicesModel{std::move(device_tree)},
    m_deviceNode(std::move(node))
{
}

void LoadDevice::undo() const
{
    auto& devplug = m_devicesModel.find();
    devplug.updateProxy.removeDevice(m_deviceNode.get<iscore::DeviceSettings>());
}

void LoadDevice::redo() const
{
    auto& devplug = m_devicesModel.find();
    devplug.updateProxy.loadDevice(m_deviceNode);
}

void LoadDevice::serializeImpl(DataStreamInput& d) const
{
    d << m_devicesModel;
    d << m_deviceNode;
}

void LoadDevice::deserializeImpl(DataStreamOutput& d)
{
    d >> m_devicesModel;
    d >> m_deviceNode;
}
