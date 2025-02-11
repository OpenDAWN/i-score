#include "CurvePointInspectorWidget.hpp"

#include <Curve/Point/CurvePointModel.hpp>
#include <Curve/CurveModel.hpp>
#include <Automation/AutomationModel.hpp>

#include <Curve/Commands/MovePoint.hpp>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>


CurvePointInspectorWidget::CurvePointInspectorWidget(
    const CurvePointModel& model,
    iscore::Document& doc,
    QWidget* parent):
    InspectorWidgetBase{model, doc, parent},
    m_model{model},
    m_dispatcher{commandDispatcher()->stack()}
{
    setObjectName("CurvePointInspectorWidget");
    setParent(parent);

    std::list<QWidget*> vec;
    auto cm = safe_cast<CurveModel*>(m_model.parent());
    auto automModel_base = dynamic_cast<AutomationModel*>(cm->parent());
    if(!automModel_base)
        return;

    auto& automModel = *automModel_base;
    m_xFactor = automModel.duration().msec();
    m_Ymin = automModel.min();
    m_yFactor = automModel.max() - m_Ymin;

    // x box
    auto widgX = new QWidget;
    auto hlayX = new QHBoxLayout{widgX};
    m_XBox = new QDoubleSpinBox{};
    m_XBox->setRange(0., m_xFactor);

    m_XBox->setValue(m_model.pos().x() * m_xFactor);

    vec.push_back(widgX);
    m_XBox->setEnabled(false);

    hlayX->addWidget(new QLabel{"t (ms)"});
    hlayX->addWidget(m_XBox);

    // y  box
    auto widgY = new QWidget;
    auto hlayY = new QHBoxLayout{widgY};
    m_YBox = new QDoubleSpinBox{};
    m_YBox->setRange(automModel.min(), automModel.max());
    m_YBox->setSingleStep(m_yFactor/100);
    m_YBox->setValue(m_model.pos().y() * m_yFactor  + m_Ymin);

    connect(m_YBox, SIGNAL(valueChanged(double)), this, SLOT(on_pointChanged(double)));

    connect(m_YBox, &QDoubleSpinBox::editingFinished,
            this, &CurvePointInspectorWidget::on_editFinished);
    vec.push_back(widgY);

    hlayY->addWidget(new QLabel{"value"});
    hlayY->addWidget(m_YBox);

    // y en %
/*    auto widgP = new QWidget;
    auto hlayP = new QHBoxLayout{widgP};
    auto spinP = new QDoubleSpinBox{};
    spinP->setRange(automModel.min(), automModel.max());
    spinP->setSingleStep(m_yFactor/100);
    spinP->setValue(m_model.pos().y());

    hlayP->addWidget(new QLabel{"value %"});
    hlayP->addWidget(spinP);

    vec.push_back(widgP);
*/
    vec.push_back(new QWidget{});

    updateAreaLayout(vec);
}

void CurvePointInspectorWidget::on_pointChanged(double d)
{
    Curve::Point pos{m_XBox->value()/m_xFactor, (m_YBox->value() - m_Ymin)/m_yFactor};
    m_dispatcher.submitCommand<MovePoint>(
                *safe_cast<CurveModel*>(m_model.parent()),
                m_model.id(),
                pos);
}

void CurvePointInspectorWidget::on_editFinished()
{
    m_dispatcher.commit();
}

