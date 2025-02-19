#pragma once
#include <iscore/plugins/panel/PanelView.hpp>

class QGraphicsScene;
class QGraphicsView;
class ScenarioBaseGraphicsView;
class DoubleSlider;
class ProcessPanelView final : public iscore::PanelView
{
        Q_OBJECT
        static const iscore::DefaultPanelStatus m_status;
    public:
        const iscore::DefaultPanelStatus& defaultPanelStatus() const override;

        ProcessPanelView(QObject* parent);

        QWidget* getWidget() override;
        const QString shortcut() const override
        { return tr("Ctrl+P"); }

        QGraphicsScene* scene() const;
        ScenarioBaseGraphicsView* view() const;

        DoubleSlider* zoomSlider() const
        { return m_zoomSlider; }

    signals:
        void sizeChanged(const QSize&);
        void horizontalZoomChanged(double d);

    private:
        QWidget* m_widget{};
        QGraphicsScene* m_scene{};
        ScenarioBaseGraphicsView* m_view{};
        DoubleSlider* m_zoomSlider{};
};
