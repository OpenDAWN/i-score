#include "RemoveLayerModelFromSlot.hpp"

#include <Scenario/Document/Constraint/Rack/Slot/SlotModel.hpp>
#include <Process/Process.hpp>
#include <Process/LayerModel.hpp>
#include <Scenario/Document/Constraint/LayerModelLoader.hpp>

using namespace iscore;
using namespace Scenario::Command;

RemoveLayerModelFromSlot::RemoveLayerModelFromSlot(
        Path<SlotModel>&& rackPath,
        const Id<LayerModel>& layerId) :
    m_path {rackPath},
    m_layerId {layerId}
{
    auto& slot = m_path.find();

    Serializer<DataStream> s{&m_serializedLayerData};
    s.readFrom(slot.layers.at(m_layerId));
}

void RemoveLayerModelFromSlot::undo() const
{
    auto& slot = m_path.find();
    Deserializer<DataStream> s {m_serializedLayerData};

    auto lm = createLayerModel(s,
                               slot.parentConstraint(),
                               &slot);
    slot.layers.add(lm);
}

void RemoveLayerModelFromSlot::redo() const
{
    auto& slot = m_path.find();
    slot.layers.remove(m_layerId);
}

void RemoveLayerModelFromSlot::serializeImpl(DataStreamInput& s) const
{
    s << m_path << m_layerId << m_serializedLayerData;
}

void RemoveLayerModelFromSlot::deserializeImpl(DataStreamOutput& s)
{
    s >> m_path >> m_layerId >> m_serializedLayerData;
}
