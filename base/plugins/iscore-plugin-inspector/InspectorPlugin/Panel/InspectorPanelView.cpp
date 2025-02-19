#include "InspectorPanelView.hpp"
#include <core/view/View.hpp>
#include <core/document/Document.hpp>
#include <QVBoxLayout>
#include <core/application/ApplicationComponents.hpp>
#include <Inspector/InspectorWidgetList.hpp>
#include "Implementation/InspectorPanel.hpp"
#include "Implementation/SelectionStackWidget.hpp"
static const iscore::DefaultPanelStatus status{true, Qt::RightDockWidgetArea, 10, QObject::tr("Inspector")};

const iscore::DefaultPanelStatus &InspectorPanelView::defaultPanelStatus() const
{ return status; }

InspectorPanelView::InspectorPanelView(iscore::View* parent) :
    iscore::PanelView {parent},
    m_widget{new QWidget}
{
    auto lay = new QVBoxLayout{m_widget};
    m_widget->setLayout(lay);
}

QWidget* InspectorPanelView::getWidget()
{
    return m_widget;
}

void InspectorPanelView::setCurrentDocument(iscore::Document* doc)
{
    using namespace iscore;

    delete m_stack;
    m_stack = nullptr;
    delete m_inspectorPanel;
    m_inspectorPanel = nullptr;

    m_currentDocument = doc;

    if(doc)
    {
        auto& appContext = doc->context().app;
        auto& fact = appContext.components.factory<InspectorWidgetList>();
        m_stack = new SelectionStackWidget{doc->selectionStack(), m_widget};
        m_inspectorPanel = new InspectorPanel{fact, doc->selectionStack(), m_widget};

        m_widget->layout()->addWidget(m_stack);
        m_widget->layout()->addWidget(m_inspectorPanel);

        setNewSelection(doc->selectionStack().currentSelection());
    }
}

void InspectorPanelView::setNewSelection(const Selection& s)
{
    m_inspectorPanel->newItemsInspected(s);
}
