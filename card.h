#ifndef CARD_H
#define CARD_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include "const.h"

class card : public QGraphicsItem
{
    public:
        card(bool = true, bool=true, char = 'A',QPointF = QPointF(0.0,0.0),qreal = 10.0);
        bool get_state()const;
        void set_state(bool);
        char get_ID()const;
        void set_ID(char);
        void set_presence(bool);
        bool get_presence()const;
        void card_swap(card*);
    private:
        bool m_presence;
        bool m_state;
        char m_ID;
        QPointF m_position;
        qreal m_length;
    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QRectF boundingRect()const;
};

#endif // CARD_H
