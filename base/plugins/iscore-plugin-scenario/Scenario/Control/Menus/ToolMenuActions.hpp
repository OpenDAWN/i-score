#pragma once
#include <Scenario/Control/Menus/ScenarioActions.hpp>

class QToolBar;
class ToolMenuActions final : public ScenarioActions
{
    public:
        ToolMenuActions(iscore::ToplevelMenuElement menuElt, ScenarioControl* parent);
        void fillMenuBar(iscore::MenubarManager *menu) override;
        void fillContextMenu(
                QMenu* menu,
                const Selection&sel,
                const TemporalScenarioPresenter& pres,
                const QPoint&,
                const QPointF&) override;
        bool populateToolBar(QToolBar* bar) override;
        void setEnabled(bool) override;

        QList<QAction*> modeActions();
        QList<QAction*> toolActions();
        QAction* shiftAction();

    private slots:
        void keyPressed(int key);
        void keyReleased(int key);

    private:
        QActionGroup* m_scenarioScaleModeActionGroup{};
        QActionGroup* m_scenarioToolActionGroup{};
        QAction* m_shiftAction{};

        QAction* m_selecttool{};
        QAction* m_createtool{};
};
