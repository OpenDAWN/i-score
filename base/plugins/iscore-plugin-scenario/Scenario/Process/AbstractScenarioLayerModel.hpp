#pragma once
#include <Scenario/Document/Constraint/ViewModels/ConstraintViewModelIdMap.hpp>
#include <Process/LayerModel.hpp>
#include <iscore/tools/ModelPath.hpp>
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
class ScenarioModel;
class ConstraintViewModel;
class ConstraintModel;
class TimeNodeModel;
class StateModel;

class EventModel;

class AbstractScenarioLayerModel : public LayerModel
{
        Q_OBJECT
    public:
        using model_type = ScenarioModel;

        virtual void makeConstraintViewModel(
                const Id<ConstraintModel>& constraintModelId,
                const Id<ConstraintViewModel>& constraintViewModelId) = 0;

        void removeConstraintViewModel(
                const Id<ConstraintViewModel>& constraintViewModelId);

        // Access to elements
        // A given constraint can be represented only once in a given scenario VM, hence...
        ConstraintViewModel& constraint(
                const Id<ConstraintModel>& constraintModelid) const;
        ConstraintViewModel& constraint(
                const Id<ConstraintViewModel>& constraintViewModelid) const;
        QVector<ConstraintViewModel*> constraints() const;

    signals:
        // "created" signal is in the relevant subclasses
        void constraintViewModelRemoved(const ConstraintViewModel&);

        void stateCreated(const StateModel&);
        void stateRemoved(const StateModel&);

        void eventCreated(const EventModel&);
        void eventRemoved(const EventModel&);

        void timeNodeCreated(const TimeNodeModel&);
        void timeNodeRemoved(const TimeNodeModel&);

        void eventMoved(const EventModel&);
        void constraintMoved(const ConstraintModel&);

    public slots:
        virtual void on_constraintRemoved(const ConstraintModel&) = 0;

    protected:
        AbstractScenarioLayerModel(const Id<LayerModel>& viewModelId,
                              const QString& name,
                              Process& sharedProcess,
                              QObject* parent) :
            LayerModel {viewModelId,
                        name,
                        sharedProcess,
                        parent
}
        {
        }

        // Copy
        AbstractScenarioLayerModel(const AbstractScenarioLayerModel& source,
                              const Id<LayerModel>& viewModelId,
                              const QString& name,
                              Process& sharedProcess,
                              QObject* parent) :
            LayerModel {viewModelId,
                        name,
                        sharedProcess,
                        parent}
        {
        }

        // Load
        template<typename Impl>
        AbstractScenarioLayerModel(Deserializer<Impl>& vis,
                              Process& sharedProcess,
                              QObject* parent) :
            LayerModel {vis,
                        sharedProcess,
                        parent}
        {
            // No data to save (the constraints vector will be rebuilt by the subclass accordingly.)
        }

        QVector<ConstraintViewModel*> m_constraints;
};


// TODO these methods should go in another file.
template<typename T>
typename T::constraint_layer_type& constraintViewModel(
        const T& scenarioViewModel,
        const Id<ConstraintViewModel>& cvm_id)
{
    return static_cast<typename T::constraint_layer_type&>(scenarioViewModel.constraint(cvm_id));
}


template<typename T>
QVector<typename T::constraint_layer_type*> constraintsViewModels(const T& scenarioViewModel)
{
    QVector<typename T::constraint_layer_type*> v;

    for(auto& elt : scenarioViewModel.constraints())
    {
        v.push_back(static_cast<typename T::constraint_layer_type*>(elt));
    }

    return v;
}

void createConstraintViewModels(const ConstraintViewModelIdMap& idMap,
                                const Id<ConstraintModel>& constraint,
                                const ScenarioModel& scenario);

// Note : the view models can also be more easily accessed using the viewModels methods of ConstraintModel
std::vector<ConstraintViewModel*> getConstraintViewModels(
        const Id<ConstraintModel>& constraintId,
        const ScenarioModel& scenario);
