#include "TimeNodeInspectorWidget.hpp"

#include "Document/TimeNode/TimeNodeModel.hpp"
#include "Document/Event/EventModel.hpp"

#include "Process/ScenarioModel.hpp"

#include <Inspector/InspectorSectionWidget.hpp>
#include "Inspector/MetadataWidget.hpp"
#include "Inspector/Event/EventWidgets/EventShortcut.hpp"

#include "Commands/TimeNode/SplitTimeNode.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QCompleter>
#include <QTreeView>
using namespace Scenario::Command;

#include "ItemModel/ScenarioItemModel.hpp"

TimeNodeInspectorWidget::TimeNodeInspectorWidget(
        const TimeNodeModel& object,
        iscore::Document& doc,
        QWidget* parent) :
    InspectorWidgetBase {object, doc, parent},
    m_model {object}
{
    setObjectName("TimeNodeInspectorWidget");
    setParent(parent);

    // default date
    QWidget* dateWid = new QWidget{this};
    QHBoxLayout* dateLay = new QHBoxLayout{dateWid};

    auto dateTitle = new QLabel{"Default date"};
    m_date = new QLabel{QString::number(m_model.date().msec()) };

    dateLay->addWidget(dateTitle);
    dateLay->addWidget(m_date);

    // Events ids list
    m_eventList = new InspectorSectionWidget{"Events", this};

    m_properties.push_back(dateWid);

    auto itm = new ScenarioItemModel(const_cast<TimeNodeModel*>(&m_model), this);
    auto view = new QTreeView;
    view->setModel(itm);
    m_properties.push_back(view);
    m_properties.push_back(m_eventList);

    updateAreaLayout(m_properties);

    // display data
    updateDisplayedValues();

    // metadata
    m_metadata = new MetadataWidget{&m_model.metadata, commandDispatcher(), &m_model, this};
    m_metadata->setType(TimeNodeModel::prettyName());

    m_metadata->setupConnections(m_model);

    addHeader(m_metadata);

    con(m_model, &TimeNodeModel::dateChanged,
        this,   &TimeNodeInspectorWidget::updateDisplayedValues);

    auto splitBtn = new QPushButton{this};
    splitBtn->setText("split timeNode");

    m_eventList->addContent(splitBtn);

    connect(splitBtn,   &QPushButton::clicked,
            this,       &TimeNodeInspectorWidget::on_splitTimeNodeClicked);
}


void TimeNodeInspectorWidget::updateDisplayedValues()
{
    // Cleanup
    // OPTIMIZEME
    for(auto& elt : m_events)
    {
        delete elt;
    }

    m_events.clear();

    m_date->setText(QString::number(m_model.date().msec()));

    for(const auto& event : m_model.events())
    {
        auto scenar = m_model.parentScenario();
        EventModel* evModel = &scenar->event(event);

        auto eventWid = new EventShortCut(QString::number((*event.val())));

        m_events.push_back(eventWid);
        m_eventList->addContent(eventWid);

        connect(eventWid, &EventShortCut::eventSelected,
                [=]()
        {
            selectionDispatcher().setAndCommit(Selection{evModel});
        });
    }
}

void TimeNodeInspectorWidget::on_splitTimeNodeClicked()
{
    QVector<Id<EventModel> > eventGroup;

    for(const auto& ev : m_events)
    {
        if(ev->isChecked())
        {
            eventGroup.push_back( Id<EventModel>(ev->eventName().toInt()));
        }
    }

    if (eventGroup.size() < int(m_events.size()))
    {
        auto cmd = new SplitTimeNode(iscore::IDocument::path(m_model),
                                     eventGroup);

        commandDispatcher()->submitCommand(cmd);
    }

    updateDisplayedValues();
}
