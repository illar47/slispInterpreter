#include "qgraphics_arc_item.hpp"

#include <cmath>

#include <QDebug>
#include <QPainter>

/*QGraphicsArcItem::QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height,
                                   QGraphicsItem *parent) {
    this->x = x; 
    this->y = y; 
    this->h = height; 
    this->w = width; 

}*/
void QGraphicsArcItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget) {
    //QGraphicsEllipseItem(x, y, w, h);
    painter->drawArc(rect(), startAngle(), spanAngle());
}
