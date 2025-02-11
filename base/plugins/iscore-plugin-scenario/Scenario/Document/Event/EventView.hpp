#pragma once
#include <QGraphicsObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <Scenario/Document/VerticalExtent.hpp>
#include "ExecutionStatus.hpp"
class EventPresenter;
class ConditionView;
class EventView final : public QGraphicsObject
{
        Q_OBJECT

    public:
        EventView(EventPresenter& presenter, QGraphicsObject* parent);
        virtual ~EventView() = default;

        int type() const override
        { return QGraphicsItem::UserType + 1; }

        const EventPresenter& presenter() const;

        QRectF boundingRect() const override
        { return {-5, -10., 10, qreal(m_extent.bottom() - m_extent.top() + 20)};  }

        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* option,
                   QWidget* widget) override;

        void setSelected(bool selected);
        bool isSelected() const;

        void setCondition(const QString& cond);
        bool hasCondition() const;

        void setTrigger(const QString& trig);
        bool hasTrigger() const;

        void setExtent(const VerticalExtent& extent);
        void setExtent(VerticalExtent&& extent);

        void setStatus(ExecutionStatus s);

    signals:
        void eventHoverEnter();
        void eventHoverLeave();

        void dropReceived(const QPointF& pos, const QMimeData*);

    public slots:
        void changeColor(QColor);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void hoverEnterEvent(QGraphicsSceneHoverEvent* h) override;
        void hoverLeaveEvent(QGraphicsSceneHoverEvent* h) override;

        void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    private:
        EventPresenter& m_presenter;
        QString m_condition;
        QString m_trigger;
        QPointF m_clickedPoint;
        QColor m_color;

        ExecutionStatus m_status{ExecutionStatus::Editing};
        bool m_selected{};

        VerticalExtent m_extent;

        ConditionView* m_conditionItem{};
};

