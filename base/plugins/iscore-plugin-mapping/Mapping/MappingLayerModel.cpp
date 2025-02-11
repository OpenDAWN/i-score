#include "MappingLayerModel.hpp"
#include "MappingModel.hpp"
#include "MappingPanelProxy.hpp"

constexpr const char MappingLayerModel::className[];
MappingLayerModel::MappingLayerModel(
        MappingModel& model,
        const Id<LayerModel>& id,
        QObject* parent) :
    LayerModel {id,
                className,
                model,
                parent}
{

}

MappingLayerModel::MappingLayerModel(
        const MappingLayerModel& source,
        MappingModel& model,
        const Id<LayerModel>& id,
        QObject* parent) :
    LayerModel {id,
                className,
                model,
                parent}
{
    // Nothing to copy
}

LayerModelPanelProxy* MappingLayerModel::make_panelProxy(QObject* parent) const
{
    return new MappingPanelProxy{*this, parent};
}

void MappingLayerModel::serialize(const VisitorVariant&) const
{
    // Nothing to save
}

const MappingModel& MappingLayerModel::model() const
{
    return static_cast<const MappingModel&>(processModel());
}
