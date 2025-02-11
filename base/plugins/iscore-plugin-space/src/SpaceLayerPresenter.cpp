#include "SpaceLayerPresenter.hpp"
#include "SpaceLayerModel.hpp"
#include "SpaceLayerView.hpp"
#include "SpaceProcess.hpp"
#include <iscore/widgets/GraphicsItem.hpp>

#include <QGraphicsScene>
#include "Widgets/SpaceGuiWindow.hpp"
#include <iscore/document/DocumentInterface.hpp>
#include <core/document/Document.hpp>
#include <QMainWindow>
SpaceLayerPresenter::SpaceLayerPresenter(const LayerModel& model, LayerView* view, QObject* parent):
    LayerPresenter{"LayerPresenter", parent},
    m_model{static_cast<const SpaceLayerModel&>(model)},
    m_view{static_cast<SpaceLayerView*>(view)},
    m_focusDispatcher{*iscore::IDocument::documentFromObject(m_model.processModel())}
{
    const SpaceProcess& procmodel = static_cast<SpaceProcess&>(m_model.processModel());
    m_spaceWindowView = new QMainWindow;
    m_spaceWindowView->setCentralWidget(
                new SpaceGuiWindow{
                    iscore::IDocument::commandStack(model),
                    procmodel,
                    m_spaceWindowView});

    connect(m_view, &SpaceLayerView::guiRequested, m_spaceWindowView, &QWidget::show);

    connect(m_view, &SpaceLayerView::contextMenuRequested,
            this, &LayerPresenter::contextMenuRequested);
    for(const auto& area : ::model(m_model).areas())
    {
        on_areaAdded(area);
    }

    con(procmodel, &SpaceProcess::areaAdded, this, &SpaceLayerPresenter::on_areaAdded);
    m_view->setEnabled(true);

    parentGeometryChanged();
}

SpaceLayerPresenter::~SpaceLayerPresenter()
{
    deleteGraphicsObject(m_view);
}

void SpaceLayerPresenter::setWidth(int width)
{
    m_view->setWidth(width);
    update();
}

void SpaceLayerPresenter::setHeight(int height)
{
    m_view->setHeight(height);
    update();
}
void SpaceLayerPresenter::on_focusChanged()
{

}

void SpaceLayerPresenter::putToFront()
{
    m_view->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
    update();
}

void SpaceLayerPresenter::putBehind()
{
    m_view->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    update();
}

void SpaceLayerPresenter::on_zoomRatioChanged(ZoomRatio)
{
    //ISCORE_TODO;
    update();
}

void SpaceLayerPresenter::parentGeometryChanged()
{
    //ISCORE_TODO;
    update();
}

const LayerModel &SpaceLayerPresenter::layerModel() const
{
    return m_model;
}

const Id<Process> &SpaceLayerPresenter::modelId() const
{
    return m_model.processModel().id();
}

void SpaceLayerPresenter::update()
{
    m_view->update();
    for(auto& pres : m_areas)
    {
        pres.update();
    }
}

#include "src/Area/AreaFactory.hpp"
#include "src/Area/SingletonAreaFactoryList.hpp"
void SpaceLayerPresenter::on_areaAdded(const AreaModel & a)
{
    auto fact = SingletonAreaFactoryList::instance().get(a.factoryKey());

    auto v = fact->makeView(m_view);
    // TODO call the factory list
    auto pres = fact->makePresenter(v, a, this);

    con(a, &AreaModel::areaChanged,
            pres, &AreaPresenter::on_areaChanged);
    m_areas.insert(pres);
    pres->on_areaChanged();
    update();
}


void SpaceLayerPresenter::fillContextMenu(
        QMenu* menu,
        const QPoint& pos,
        const QPointF& scenepos) const
{
    auto act = menu->addAction(tr("Show"));
    connect(act, &QAction::triggered, this, [&] () {
       m_spaceWindowView->show();
    });
}
