#include "StringValueWidget.hpp"
#include <iscore/widgets/MarginLess.hpp>
#include <QLineEdit>
#include <QGridLayout>

StringValueWidget::StringValueWidget(const QString &value, QWidget *parent)
    : ValueWidget{parent}
{
    auto lay = new iscore::MarginLess<QGridLayout>;
    m_value = new QLineEdit;
    lay->addWidget(m_value);
    m_value->setText(value);
    this->setLayout(lay);
}

iscore::Value StringValueWidget::value() const
{
    return iscore::Value{m_value->text()};
}
