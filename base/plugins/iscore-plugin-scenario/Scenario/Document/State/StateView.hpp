#pragma once
#include <QGraphicsObject>

class QMimeData;
class StatePresenter;

class StateView final : public QGraphicsObject
{
        Q_OBJECT
    public:
        StateView(StatePresenter &presenter, QGraphicsItem *parent = 0);
        virtual ~StateView() = default;

        int type() const override
        { return QGraphicsItem::UserType + 4; }

        const StatePresenter& presenter() const;

        QRectF boundingRect() const override
        { return {-m_radiusFull, -m_radiusFull, 2*m_radiusFull, 2*m_radiusFull }; }

        void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

        void setContainMessage(bool);
        void setSelected(bool arg);

        void changeColor(const QColor&);

    signals:
        void dropReceived(const QMimeData*);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    private:
        StatePresenter& m_presenter;

        bool m_containMessage{false};
        bool m_selected{false};

        QColor m_baseColor;

        static const constexpr qreal m_radiusFull = 7.;
        static const constexpr qreal m_radiusVoid = 3.;

};
