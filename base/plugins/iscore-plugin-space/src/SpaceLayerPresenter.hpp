#pragma once
#include <Process/LayerPresenter.hpp>
#include <Process/Focus/FocusDispatcher.hpp>
#include <iscore/tools/IdentifiedObjectMap.hpp>

#include "Area/AreaModel.hpp"
#include "Area/AreaPresenter.hpp"
class QMainWindow;
class LayerView;
class SpaceLayerModel;
class SpaceLayerView;
class SpaceLayerPresenter : public LayerPresenter
{
    public:
        SpaceLayerPresenter(const LayerModel& model,
                            LayerView* view,
                            QObject* parent);
        ~SpaceLayerPresenter();

        void setWidth(int width) override;
        void setHeight(int height) override;

        void on_focusChanged() override;

        void putToFront() override;
        void putBehind() override;

        void on_zoomRatioChanged(ZoomRatio) override;
        void parentGeometryChanged() override;

        const LayerModel &layerModel() const override;
        const Id<Process> &modelId() const override;

        void update();

    private:
        void on_areaAdded(const AreaModel&);

        const SpaceLayerModel& m_model;
        SpaceLayerView* m_view;

        QMainWindow* m_spaceWindowView{};
        IdContainer<AreaPresenter, AreaModel> m_areas;
        FocusDispatcher m_focusDispatcher;


        // LayerPresenter interface
    public:
        void fillContextMenu(QMenu*, const QPoint& pos, const QPointF& scenepos) const override;
};
