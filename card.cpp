#include "card.h"


card::card(bool k, bool e, char b, QPointF c, qreal d)
{
    m_presence=k;
    m_state=e;
    m_ID=b;
    m_position=c;
    m_length=d;
    setPos(m_position);
}

bool card::get_state()const
{
    return m_state;
}

void card::set_state(bool m)
{
    m_state = m;
}

char card::get_ID()const
{
    return m_ID;
}

void card::set_ID(char m)
{
    m_ID = m;
}

void card::set_presence(bool m)
{
    m_presence=m;
}

bool card::get_presence() const
{
    return m_presence;
}

void card::card_swap(card *a)
{
    swap(m_presence,a->m_presence);
    swap(m_ID, a->m_ID);
    swap(m_state, a->m_state);
}

void card::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QFont f("Sans",m_length/2.0,m_length/2.0);
    QBrush br;
    QPen p;
    if (m_presence == PRESENT)
    {
        p.setWidth(m_length/15.0);
        p.setColor(Qt::blue);
        painter->setPen(p);
        if (m_state == OPENED)
        {
            painter->save();
            if (m_ID>='a' && m_ID<='z')
                p.setColor(Qt::red);
            painter->setPen(p);
            painter->setFont(f);
            painter->drawText(m_length*0.25,m_length*0.75,QString(m_ID));
            painter->restore();
        }
        else
        {
            br.setColor(Qt::blue);
            br.setStyle(Qt::DiagCrossPattern);
            painter->setBrush(br);
        }
    }
    else
    {
        br.setColor(Qt::black);
        br.setStyle(Qt::SolidPattern);
        painter->setBrush(br);
    }
    painter->drawRect(boundingRect());
}

QRectF card::boundingRect()const
{
    return QRectF(0.0, 0.0, m_length,m_length);
}

