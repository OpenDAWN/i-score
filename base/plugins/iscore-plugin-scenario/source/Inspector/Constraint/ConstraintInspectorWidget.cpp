#include "ConstraintInspectorWidget.hpp"

#include "DialogWidget/AddProcessDialog.hpp"
#include "Widgets/BoxWidget.hpp"
#include "Widgets/DurationSectionWidget.hpp"
#include "Widgets/Box/BoxInspectorSection.hpp"

#include "Document/Constraint/ConstraintModel.hpp"
#include "Document/Constraint/ViewModels/Temporal/TemporalConstraintViewModel.hpp"
#include "Document/Constraint/Box/BoxModel.hpp"
#include "Document/Constraint/Box/Deck/DeckModel.hpp"
#include "Commands/Constraint/AddProcessToConstraint.hpp"
#include "Commands/Constraint/AddProcessViewInNewDeck.hpp"
#include "Commands/Constraint/AddBoxToConstraint.hpp"
#include "Document/Event/EventModel.hpp"
#include "Commands/Scenario/ShowBoxInViewModel.hpp"
#include "Commands/Scenario/HideBoxInViewModel.hpp"
#include "ProcessInterface/ProcessModel.hpp"

#include "Inspector/MetadataWidget.hpp"
#include <Inspector/InspectorWidgetList.hpp>
#include "Document/BaseElement/BaseElementPresenter.hpp"
#include "Process/ScenarioModel.hpp"

#include "core/document/DocumentModel.hpp"

#include <Inspector/Separator.hpp>
#include <QFrame>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QFormLayout>
#include <QToolButton>
#include <QPushButton>

using namespace Scenario::Command;
using namespace iscore;
using namespace iscore::IDocument;




ConstraintInspectorWidget::ConstraintInspectorWidget(
        const ConstraintModel* object,
        QWidget* parent) :
    InspectorWidgetBase(object, parent)
{
    setObjectName("Constraint");
    setInspectedObject(object);
    m_currentConstraint = object;

    ////// HEADER
    // metadata
    m_metadata = new MetadataWidget{&object->metadata, commandDispatcher(), object, this};
    m_metadata->setType(ConstraintModel::prettyName());

    m_metadata->setupConnections(object);

    addHeader(m_metadata);


    ////// BODY
    QPushButton* setAsDisplayedConstraint = new QPushButton {"Full view", this};
    connect(setAsDisplayedConstraint, &QPushButton::clicked,
            [this]()
    {
        auto& base = get<BaseElementPresenter> (*documentFromObject(m_currentConstraint));

        base.setDisplayedConstraint(this->model());
    });

    m_properties.push_back(setAsDisplayedConstraint);

    // Events
    if(auto scenario = qobject_cast<ScenarioModel*>(m_currentConstraint->parent()))
    {
        m_properties.push_back(makeEventWidget(scenario));
    }

    // Separator
    m_properties.push_back(new Separator {this});

    // Durations
    m_durationSection = new DurationSectionWidget {this};
    m_properties.push_back(m_durationSection);

    // Separator
    m_properties.push_back(new Separator {this});

    // Processes
    m_processSection = new InspectorSectionWidget("Processes", this);
    m_processSection->setObjectName("Processes");

    m_properties.push_back(m_processSection);

        QWidget* addProc = new QWidget(this);
        QHBoxLayout* addProcLayout = new QHBoxLayout;
        addProcLayout->setContentsMargins(0, 0, 0 , 0);
        addProc->setLayout(addProcLayout);
        // Button
        QToolButton* addProcButton = new QToolButton;
        addProcButton->setText("+");
        addProcButton->setObjectName("addAProcess");

        // Text
        auto addProcText = new QLabel("Add Process");
        addProcText->setStyleSheet(QString("text-align : left;"));

        addProcLayout->addWidget(addProcButton);
        addProcLayout->addWidget(addProcText);
        auto addProcess = new AddProcessDialog {this};

        connect(addProcButton,  &QToolButton::pressed,
                addProcess, &AddProcessDialog::launchWindow);

    m_properties.push_back(addProc);

    connect(addProcess, &AddProcessDialog::okPressed,
            this, &ConstraintInspectorWidget::createProcess);

    // Separator
    m_properties.push_back(new Separator {this});

    // Boxes
    m_boxSection = new InspectorSectionWidget {"Boxes", this};
    m_boxSection->setObjectName("Boxes");
    m_boxSection->expand();

    m_boxWidget = new BoxWidget {this};

    m_properties.push_back(m_boxSection);
    m_properties.push_back(m_boxWidget);

    // Plugins
    iscore::Document* doc = iscore::IDocument::documentFromObject(object);

    for(auto& plugdata : object->pluginModelList().list())
    {
        for(iscore::DocumentDelegatePluginModel* plugin : doc->model()->pluginModels())
        {
            auto md = plugin->makeElementPluginWidget(plugdata, this);
            if(md)
            {
                m_properties.push_back(md);
                break;
            }
        }
    }

    updateDisplayedValues(object);

    // Display data
    updateAreaLayout(m_properties);
}

const ConstraintModel* ConstraintInspectorWidget::model() const
{
    return m_currentConstraint;
}

void ConstraintInspectorWidget::updateDisplayedValues(const ConstraintModel* constraint)
{
    // Cleanup the widgets
    for(auto& process : m_processesSectionWidgets)
    {
        m_processSection->removeContent(process);
    }

    m_processesSectionWidgets.clear();

    for(auto& box_pair : m_boxesSectionWidgets)
    {
        m_boxSection->removeContent(box_pair.second);
    }

    m_boxesSectionWidgets.clear();

    // Cleanup the connections
    for(auto& connection : m_connections)
    {
        QObject::disconnect(connection);
    }

    m_connections.clear();


    if(constraint != nullptr)
    {
        m_currentConstraint = constraint;

        // Constraint interface
        m_connections.push_back(
            connect(model(),	&ConstraintModel::processCreated,
                    this,		&ConstraintInspectorWidget::on_processCreated));
        m_connections.push_back(
            connect(model(),	&ConstraintModel::processRemoved,
                    this,		&ConstraintInspectorWidget::on_processRemoved));
        m_connections.push_back(
            connect(model(),	&ConstraintModel::boxCreated,
                    this,		&ConstraintInspectorWidget::on_boxCreated));
        m_connections.push_back(
            connect(model(),	&ConstraintModel::boxRemoved,
                    this,		&ConstraintInspectorWidget::on_boxRemoved));

        m_connections.push_back(
            connect(model(), &ConstraintModel::viewModelCreated,
                    this,    &ConstraintInspectorWidget::on_constraintViewModelCreated));
        m_connections.push_back(
            connect(model(), &ConstraintModel::viewModelRemoved,
                    this,    &ConstraintInspectorWidget::on_constraintViewModelRemoved));

        // Processes
        for(ProcessModel* process : model()->processes())
        {
            displaySharedProcess(process);
        }

        // Box
        m_boxWidget->setModel(model());

        for(BoxModel* box : model()->boxes())
        {
            setupBox(box);
        }
    }
    else
    {
        m_currentConstraint = nullptr;
        m_boxWidget->setModel(nullptr);
    }
}

void ConstraintInspectorWidget::createProcess(QString processName)
{
    auto cmd = new AddProcessToConstraint
    {
        iscore::IDocument::path(model()),
        processName
    };
    emit commandDispatcher()->submitCommand(cmd);
}

void ConstraintInspectorWidget::createBox()
{
    auto cmd = new AddBoxToConstraint(
        iscore::IDocument::path(model()));
    emit commandDispatcher()->submitCommand(cmd);
}

void ConstraintInspectorWidget::createProcessViewInNewDeck(QString processName)
{
    // TODO this will bite us when the name does not contain the id anymore.
    // We will have to stock the id's somewhere.
    auto cmd = new AddProcessViewInNewDeck(
        iscore::IDocument::path(model()),
        id_type<ProcessModel>(processName.toInt()));

    emit commandDispatcher()->submitCommand(cmd);
}

void ConstraintInspectorWidget::activeBoxChanged(QString box, AbstractConstraintViewModel* vm)
{
    // TODO mettre à jour l'inspecteur si la box affichée change (i.e. via une commande réseau).
    if (m_boxWidget == 0)
        return;

    if(box == m_boxWidget->hiddenText)
    {
        if(vm->isBoxShown())
        {
            auto cmd = new HideBoxInViewModel(vm);
            emit commandDispatcher()->submitCommand(cmd);
        }
    }
    else
    {
        bool ok {};
        auto id = id_type<BoxModel> (box.toInt(&ok));

        if(ok)
        {
            auto cmd = new ShowBoxInViewModel(vm, id);
            emit commandDispatcher()->submitCommand(cmd);
        }
    }
}

#include "Commands/Constraint/RemoveProcessFromConstraint.hpp"
void ConstraintInspectorWidget::displaySharedProcess(ProcessModel* process)
{
    InspectorSectionWidget* newProc = new InspectorSectionWidget(process->processName());

    // Process
    auto processWidget = InspectorWidgetList::makeInspectorWidget(process->processName(), process, newProc);
    newProc->addContent(processWidget);

    // Start & end state
    QWidget* stateWidget = new QWidget;
    QFormLayout* stateLayout = new QFormLayout;
    stateLayout->setSpacing(0);
    stateLayout->setContentsMargins(0, 0, 0, 0);
    stateWidget->setLayout(stateLayout);

    if(auto start = process->startState())
    {
        auto startWidg = InspectorWidgetList::makeInspectorWidget(start->stateName(), start, newProc);
        stateLayout->addRow(tr("Start state"), startWidg);
    }

    if(auto end = process->endState())
    {
        auto endWidg = InspectorWidgetList::makeInspectorWidget(end->stateName(), end, newProc);
        stateLayout->addRow(tr("End state"), endWidg);
    }
    newProc->addContent(stateWidget);

    // Delete button
    auto deleteButton = new QPushButton{"Delete"};
    connect(deleteButton, &QPushButton::pressed, this, [=] ()
    {
        auto cmd = new RemoveProcessFromConstraint{iscore::IDocument::path(model()), process->id()};
        emit commandDispatcher()->submitCommand(cmd);
    });
    newProc->addContent(deleteButton);

    // Global setup
    m_processesSectionWidgets.push_back(newProc);
    m_processSection->addContent(newProc);

    connect(processWidget,   SIGNAL(createViewInNewDeck(QString)),
            this,   SLOT(createProcessViewInNewDeck(QString)));
}

void ConstraintInspectorWidget::setupBox(BoxModel* box)
{
    // Display the widget
    BoxInspectorSection* newBox = new BoxInspectorSection {QString{"Box.%1"} .arg(*box->id().val()),
                                                           box,
                                                           this
                                                          };

    m_boxesSectionWidgets[box->id()] = newBox;
    m_boxSection->addContent(newBox);
}

QWidget* ConstraintInspectorWidget::makeEventWidget(ScenarioModel* scenar)
{
    QWidget* eventWid = new QWidget{this};
    QFormLayout* eventLay = new QFormLayout {eventWid};
    eventLay->setVerticalSpacing(0);

    QPushButton* start = new QPushButton{tr("None")};
    start->setStyleSheet ("text-align: left");
    QPushButton* end = new QPushButton {tr("None")};
    end->setStyleSheet ("text-align: left");
    start->setFlat(true);
    end->setFlat(true);

    auto sev = m_currentConstraint->startEvent();
    auto eev = m_currentConstraint->endEvent();
    if(sev)
    {
        start->setText(QString::number(*sev.val()));

        connect(start, &QPushButton::clicked,
                [=]() {
            selectionDispatcher()->setAndCommit(Selection{&scenar->event(sev)});
        });
    }

    if(eev)
    {
        end->setText(QString::number(*eev.val()));
        connect(end, &QPushButton::clicked,
                [=]()
        {
            selectionDispatcher()->setAndCommit(Selection{&scenar->event(eev)});
        });
    }

    eventLay->addRow(tr("Start Event"), start);
    eventLay->addRow(tr("End Event"), end);

    return eventWid;
}

void ConstraintInspectorWidget::on_processCreated(
        QString processName,
        id_type<ProcessModel> processId)
{
    reloadDisplayedValues();
}

void ConstraintInspectorWidget::on_processRemoved(id_type<ProcessModel> processId)
{
    reloadDisplayedValues();
}


void ConstraintInspectorWidget::on_boxCreated(id_type<BoxModel> boxId)
{
    setupBox(model()->box(boxId));
    m_boxWidget->viewModelsChanged();
}

void ConstraintInspectorWidget::on_boxRemoved(id_type<BoxModel> boxId)
{
    auto ptr = m_boxesSectionWidgets[boxId];
    m_boxesSectionWidgets.erase(boxId);

    if(ptr)
    {
        ptr->deleteLater();
    }

    m_boxWidget->viewModelsChanged();
}

void ConstraintInspectorWidget::on_constraintViewModelCreated(id_type<AbstractConstraintViewModel> cvmId)
{
    qDebug() << "Created";
    m_boxWidget->viewModelsChanged();
}

void ConstraintInspectorWidget::on_constraintViewModelRemoved(id_type<AbstractConstraintViewModel> cvmId)
{
    qDebug() << "Removed";
    m_boxWidget->viewModelsChanged();
}
