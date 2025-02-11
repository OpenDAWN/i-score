#pragma once
#include <Inspector/InspectorSectionWidget.hpp>
#include <iscore/tools/SettableIdentifier.hpp>
#include <unordered_map>
class RackModel;
class SlotModel;
class AddSlotWidget;
class ConstraintInspectorWidget;
class SlotInspectorSection;

// Contains a single rack which can contain multiple slots and a Add Slot button.
class RackInspectorSection final : public InspectorSectionWidget
{
    public:
        RackInspectorSection(
                const QString& name,
                const RackModel& model,
                ConstraintInspectorWidget* parent);

        void addSlotInspectorSection(const SlotModel&);
        void createSlot();

        auto constraintInspector() const
        { return m_parent; }

    private:
        ConstraintInspectorWidget* m_parent{};

        void on_slotCreated(const SlotModel&);
        void on_slotRemoved(const SlotModel&);
        const RackModel& m_model;

        InspectorSectionWidget* m_slotSection {};
        AddSlotWidget* m_slotWidget {};

        std::unordered_map<Id<SlotModel>, SlotInspectorSection*, id_hash<SlotModel>> slotmodelsSectionWidgets;
};
