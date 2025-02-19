#include "TriggerPresenter.hpp"
#include "TriggerModel.hpp"
#include "TriggerView.hpp"

TriggerPresenter::TriggerPresenter(
        const TriggerModel& model,
        QGraphicsObject* parentView,
        QObject* parent):
    QObject{parent},
    m_model{model},
    m_view{new TriggerView{parentView}}
{
    m_view->setVisible(m_model.active());
    m_view->setPos(-7.5, -25.);
    con(m_model, &TriggerModel::activeChanged,
            this, [&] ()
    {
        m_view->setVisible(m_model.active());
    });

    connect(m_view, &TriggerView::pressed,
            &m_model, &TriggerModel::triggered);
}

