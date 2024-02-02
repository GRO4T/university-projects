#include "inc/gui/container.h"
#include <QDebug>

#include <QPainter>
#include <qnamespace.h>

Container::Container(const QPointF& pos, const QPointF& size, QGraphicsItem* parent)
    : QGraphicsRectItem(parent) {
    QGraphicsRectItem::setPos(pos.x(), pos.y());
    set_size(size);
}

void Container::set_type(Type type) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    switch (type) {
        case Type::C: brush.setColor(Qt::GlobalColor::cyan); break;
        case Type::M: brush.setColor(Qt::GlobalColor::magenta); break;
        case Type::Y: brush.setColor(Qt::GlobalColor::yellow); break;
        case Type::K: brush.setColor(Qt::GlobalColor::black); break;
    default:
        brush.setColor(Qt::GlobalColor::white);
    }
    QGraphicsRectItem::setBrush(brush);
}
    
void Container::set_size(const QPointF& size) {
    QGraphicsRectItem::setRect(0, 0, size.x(), size.y());
}

QPointF Container::get_size() const noexcept {
    const auto & bounding = QGraphicsRectItem::rect();
    return QPointF(bounding.x(), bounding.y());
}
