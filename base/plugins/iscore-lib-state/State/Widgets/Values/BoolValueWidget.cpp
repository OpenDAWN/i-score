#include "BoolValueWidget.hpp"

#include <iscore/widgets/MarginLess.hpp>

#include <QComboBox>
#include <QGridLayout>

BoolValueWidget::BoolValueWidget(bool value, QWidget *parent)
    : ValueWidget{parent}
{
    auto lay = new iscore::MarginLess<QGridLayout>;
    m_value = new QComboBox;
    m_value->addItems({tr("false"), tr("true")});

    lay->addWidget(m_value);
    m_value->setCurrentIndex(value ? 1 : 0);
    this->setLayout(lay);
}

iscore::Value BoolValueWidget::value() const
{
    return iscore::Value{bool(m_value->currentIndex())};
}
