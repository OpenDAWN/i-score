#include "AddProcessToConstraint.hpp"
#include "AddLayerInNewSlot.hpp"
#include "Rack/Slot/AddLayerModelToSlot.hpp"

#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Document/Constraint/Rack/RackModel.hpp>
#include <Scenario/Document/Constraint/Rack/Slot/SlotModel.hpp>

#include <Process/Process.hpp>

#include <Process/ProcessList.hpp>
#include <Process/ProcessFactory.hpp>

#include <iscore/document/DocumentInterface.hpp>
#include <iscore/tools/SettableIdentifierGeneration.hpp>
#include <iscore/tools/std/StdlibWrapper.hpp>
#include <core/application/ApplicationComponents.hpp>
#include <Scenario/Document/Constraint/ViewModels/ConstraintViewModel.hpp>

using namespace iscore;
using namespace Scenario::Command;


AddProcessToConstraint::AddProcessToConstraint(
        Path<ConstraintModel>&& constraintPath,
        const ProcessFactoryKey& process) :
    m_path {std::move(constraintPath) },
    m_processName {process}
{
    auto& fact = context.components.factory<DynamicProcessList>();

    auto& constraint = m_path.find();
    m_createdProcessId = getStrongId(constraint.processes);
    m_noRackes = (constraint.racks.empty() && constraint.objectName() != "BaseConstraintModel" );
    m_notBaseConstraint = (constraint.objectName() != "BaseConstraintModel");

    if(m_noRackes)
    {
        m_createdRackId = getStrongId(constraint.racks);
        m_createdSlotId = Id<SlotModel>(iscore::id_generator::getFirstId());
        m_createdLayerId = Id<LayerModel> (iscore::id_generator::getFirstId());
        m_layerConstructionData = fact.list().get(m_processName)->makeStaticLayerConstructionData();
    }
    else if (m_notBaseConstraint)
    {
        ISCORE_ASSERT(!constraint.racks.empty());
        const auto& firstRack = *constraint.racks.begin();
        if(!firstRack.slotmodels.empty())
        {
            const auto& firstSlotModel = *firstRack.slotmodels.begin();

            m_layerConstructionData = fact.list().get(m_processName)->makeStaticLayerConstructionData();
            m_createdLayerId = getStrongId(firstSlotModel.layers);
        }
    }
    else
    {
        // Base constraint : add in new slot?
    }
}

void AddProcessToConstraint::undo() const
{
    auto& constraint = m_path.find();
    if(m_noRackes)
    {
        auto& rack = constraint.racks.at(m_createdRackId);

        // Removing the slot will remove the layer
        rack.slotmodels.remove(m_createdSlotId);
        constraint.racks.remove(m_createdRackId);
    }
    else if(m_notBaseConstraint)
    {
        ISCORE_ASSERT(!constraint.racks.empty());
        ISCORE_ASSERT(!(*constraint.racks.begin()).slotmodels.empty());

        auto& slot = *(*constraint.racks.begin()).slotmodels.begin();
        slot.layers.remove(m_createdLayerId);
    }

    constraint.processes.remove(m_createdProcessId);
}

void AddProcessToConstraint::redo() const
{
    auto& fact = context.components.factory<DynamicProcessList>();
    auto& constraint = m_path.find();

    // Create process model
    auto proc =
            fact.list().get(m_processName)
              ->makeModel(
                constraint.duration.defaultDuration(),
                m_createdProcessId,
                &constraint);

    constraint.processes.add(proc);
    if(m_noRackes)
    {
        // TODO refactor with AddRackToConstraint
        auto rack = new RackModel{m_createdRackId, &constraint};
        constraint.racks.add(rack);

        // If it is the first rack created,
        // it is also assigned to all the views of the constraint.
        if(constraint.racks.size() == 1)
        {
            for(const auto& vm : constraint.viewModels())
            {
                vm->showRack(m_createdRackId);
            }
        }

        // Slot
        rack->addSlot(new SlotModel {m_createdSlotId,
                                    rack});

        // Process View
        auto& slot = rack->slotmodels.at(m_createdSlotId);

        slot.layers.add(proc->makeLayer(m_createdLayerId, m_layerConstructionData, &slot));
    }
    else if(m_notBaseConstraint)
    {
        ISCORE_ASSERT(!constraint.racks.empty());
        ISCORE_ASSERT(!(*constraint.racks.begin()).slotmodels.empty());

        auto& slot = *(*constraint.racks.begin()).slotmodels.begin();

        slot.layers.add(proc->makeLayer(m_createdLayerId, m_layerConstructionData, &slot));
    }
}

void AddProcessToConstraint::serializeImpl(DataStreamInput& s) const
{
    s << m_path
      << m_processName
      << m_createdProcessId
      << m_createdRackId
      << m_createdSlotId
      << m_createdLayerId
      << m_layerConstructionData
      << m_noRackes
      << m_notBaseConstraint;
}

void AddProcessToConstraint::deserializeImpl(DataStreamOutput& s)
{
    s >> m_path
      >> m_processName
      >> m_createdProcessId
      >> m_createdRackId
      >> m_createdSlotId
      >> m_createdLayerId
      >> m_layerConstructionData
      >> m_noRackes
      >> m_notBaseConstraint;
}
