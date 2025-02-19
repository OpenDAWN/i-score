#pragma once

#include <QMap>
#include <QPointer>
#include "Scenario/Process/AbstractScenarioLayerModel.hpp"
class ScenarioModel;
class RackModel;
class TemporalConstraintViewModel;
class TemporalScenarioPresenter;

class ConstraintModel;

namespace Scenario
{
class ToolPalette;
}
class TemporalScenarioLayerModel final : public AbstractScenarioLayerModel
{
        Q_OBJECT
    public:
        using model_type = ScenarioModel;
        using constraint_layer_type = TemporalConstraintViewModel;

        TemporalScenarioLayerModel(const Id<LayerModel>& id,
                              const QMap<Id<ConstraintModel>,
                              Id<ConstraintViewModel>>& constraintIds,
                              ScenarioModel& model,
                              QObject* parent);

        // Copy
        TemporalScenarioLayerModel(const TemporalScenarioLayerModel& source,
                              const Id<LayerModel>& id,
                              ScenarioModel& model,
                              QObject* parent);

        // Load
        template<typename Impl>
        TemporalScenarioLayerModel(Deserializer<Impl>& vis,
                              ScenarioModel& model,
                              QObject* parent) :
            AbstractScenarioLayerModel {vis, model, parent}
        {
            vis.writeTo(*this);
        }

        LayerModelPanelProxy* make_panelProxy(QObject* parent) const override;

        ~TemporalScenarioLayerModel() = default;

        void serialize(const VisitorVariant&) const override;

        void makeConstraintViewModel(
                const Id<ConstraintModel>& constraintModelId,
                const Id<ConstraintViewModel>& constraintViewModelId) override;

        void addConstraintViewModel(constraint_layer_type* constraint_view_model);

    signals:
        void constraintViewModelCreated(const TemporalConstraintViewModel&);

    public:
        void on_constraintRemoved(const ConstraintModel&) override;

};
