#ifndef CONTAINER_H
#define CONTAINER_H

#include <QGraphicsItem>
#include <QBrush>
#include <QPointF>

// QGraphicsObject
class Container :public QObject, public QGraphicsRectItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    Q_PROPERTY(QPointF position READ pos WRITE setPos)

public:
    enum class Type {
        C,
        M,
        Y,
        K,
        None
    };

    Container(const QPointF& pos, const QPointF& size, QGraphicsItem* parent = nullptr);
    
    void set_type(Type type);

    void set_size(const QPointF& size);
    QPointF get_size() const noexcept;
};

#endif // CONTAINER_H
