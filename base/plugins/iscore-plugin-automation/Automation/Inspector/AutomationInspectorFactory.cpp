#include "AutomationInspectorFactory.hpp"
#include "AutomationInspectorWidget.hpp"
#include <Automation/AutomationModel.hpp>

//using namespace iscore;

InspectorWidgetBase* AutomationInspectorFactory::makeWidget(
        const QObject& sourceElement,
        iscore::Document& doc,
        QWidget* parent)
{
    return new AutomationInspectorWidget{
                safe_cast<const AutomationModel&>(sourceElement),
                doc,
                parent};
}

const QList<QString>& AutomationInspectorFactory::key_impl() const
{
    static const QList<QString> lst{AutomationProcessMetadata::processObjectName()};
    return lst;
}
