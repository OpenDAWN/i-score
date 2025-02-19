#include "ClickableLabelItem.hpp"
#include <Process/Style/ProcessFonts.hpp>
#include <Scenario/Document/Constraint/ViewModels/ConstraintHeader.hpp>
#include <QBrush>

SeparatorItem::SeparatorItem(QGraphicsItem *parent):
    QGraphicsSimpleTextItem{"/", parent}
{
    auto font = ProcessFonts::Sans();
    font.setPointSize(10);
    font.setBold(true);
    this->setFont(font);
    this->setBrush(Qt::white);
}


ClickableLabelItem::ClickableLabelItem(
        ClickHandler&& onClick,
        const QString &text,
        QGraphicsItem *parent):
    QGraphicsSimpleTextItem{text, parent},
    m_onClick{std::move(onClick)}
{
    auto font = ProcessFonts::Sans();
    font.setPointSize(10);
    font.setBold(true);
    this->setFont(font);
    this->setBrush(Qt::white);

    this->setAcceptHoverEvents(true);
}


void ClickableLabelItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_onClick(this);
}


void ClickableLabelItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(Qt::blue);
    update();
}

void ClickableLabelItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(Qt::white);
    update();
}

int ClickableLabelItem::index() const
{
    return m_index;
}

void ClickableLabelItem::setIndex(int index)
{
    m_index = index;
}
