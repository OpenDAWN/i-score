#pragma once
#include <QGraphicsObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include "EventHalves.hpp"
class EventPresenter;
class ConditionView;
class TriggerView;
class EventView : public QGraphicsObject
{
        Q_OBJECT

    public:
        EventView(EventPresenter& presenter, QGraphicsObject* parent);
        virtual ~EventView() = default;

        int type() const override;

        const EventPresenter& presenter() const;

        QRectF boundingRect() const override;
        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* option,
                   QWidget* widget) override;

        void setSelected(bool selected);

        bool isSelected() const;

        bool isShadow() const;

        void setCondition(const QString& cond);
        bool hasCondition() const;

        void setTrigger(const QString& trig);
        bool hasTrigger() const;

        void setExtremities(int top, int bottom);
        void addPoint(int newY);


    signals:
        void eventHoverEnter();
        void eventHoverLeave();

        void dropReceived(const QMimeData*);

    public slots:
        void changeColor(QColor);
        void setShadow(bool arg);

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* h) override;
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* h) override;

        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

    private:
        EventPresenter& m_presenter;
        QString m_condition;
        QString m_trigger;
        QPointF m_clickedPoint;
        QColor m_color;

        bool m_shadow {false};
        bool m_selected{};

        int m_top {0};
        int m_bottom {0};

        ConditionView* m_conditionItem{};
        TriggerView* m_triggerItem{};
};

