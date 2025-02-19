#include "EventPresenter.hpp"

#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Document/Event/EventView.hpp>

#include <Scenario/Commands/Event/State/AddStateWithData.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <core/document/Document.hpp>
#include <State/StateMimeTypes.hpp>
#include <State/MessageListSerialization.hpp>

#include <iscore/widgets/GraphicsItem.hpp>
#include <QMimeData>
#include <QJsonDocument>
#include <QGraphicsScene>
EventPresenter::EventPresenter(const EventModel& model,
                               QGraphicsObject* parentview,
                               QObject* parent) :
    NamedObject {"EventPresenter", parent},
    m_model {model},
    m_view {new EventView{*this, parentview}},
    m_dispatcher{iscore::IDocument::commandStack(m_model)}
{
    // The scenario catches this :
    con(m_model.selection, &Selectable::changed,
            m_view, &EventView::setSelected);

    con((m_model.metadata),  &ModelMetadata::colorChanged,
            m_view,                 &EventView::changeColor);

    con(m_model, &EventModel::statusChanged,
        m_view,  &EventView::setStatus);

    connect(m_view, &EventView::eventHoverEnter,
            this,   &EventPresenter::eventHoverEnter);

    connect(m_view, &EventView::eventHoverLeave,
            this,   &EventPresenter::eventHoverLeave);

    connect(m_view, &EventView::dropReceived,
            this, &EventPresenter::handleDrop);

    m_view->setCondition(m_model.condition().toString());

    con(m_model, &EventModel::conditionChanged,
        this, [&] (const iscore::Condition& c) { m_view->setCondition(c.toString()); });
}

EventPresenter::~EventPresenter()
{
    deleteGraphicsObject(m_view);
}

const Id<EventModel>& EventPresenter::id() const
{
    return m_model.id();
}

EventView* EventPresenter::view() const
{
    return m_view;
}

const EventModel& EventPresenter::model() const
{
    return m_model;
}

bool EventPresenter::isSelected() const
{
    return m_model.selection.get();
}

void EventPresenter::triggerSetted(QString trig)
{
    m_view->setTrigger(trig);
}

void EventPresenter::handleDrop(const QPointF& pos, const QMimeData *mime)
{
    // We don't want to create a new state in BaseScenario
    auto scenar = dynamic_cast<ScenarioModel*>(m_model.parentScenario());
    // todo Maybe the drop should be handled by the scenario presenter ?? or not

    // If the mime data has states in it we can handle it.
    if(scenar && mime->formats().contains(iscore::mime::messagelist()))
    {
        Mime<iscore::MessageList>::Deserializer des{*mime};
        iscore::MessageList ml = des.deserialize();

        auto cmd = new Scenario::Command::AddStateWithData{
                *scenar,
                m_model.id(),
                pos.y() / m_view->parentItem()->boundingRect().size().height(),
                std::move(ml)};
        m_dispatcher.submitCommand(cmd);
    }
}

