#pragma once

#include <Inspector/InspectorWidgetBase.hpp>

class ScenarioModel;
class ScenarioInspectorWidget final : public InspectorWidgetBase
{
        Q_OBJECT
    public:
        explicit ScenarioInspectorWidget(
                const ScenarioModel& object,
                iscore::Document& doc,
                QWidget* parent);

    signals:
        void createViewInNewSlot(QString); // TODO make a ProcessInspectorWidget

    private:
        const ScenarioModel& m_model;
};
