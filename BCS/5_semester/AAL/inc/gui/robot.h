#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsItemGroup>
#include <QSequentialAnimationGroup>
#include <QPointF>
#include <vector>
#include <memory>

#include "inc/gui/container.h"

class CMYKRobot :public QObject, public QGraphicsItemGroup {
    Q_OBJECT

    Q_PROPERTY(QPointF margin READ margin WRITE setMargin)
    Q_PROPERTY(QPointF containerSize READ containerSize WRITE setContainerSize)

    QSequentialAnimationGroup animation_group;
    std::vector<std::unique_ptr<Container>> containers;

    QPointF margin_size;
    QPointF container_size;
    std::string initial_containers_state;
    std::vector<std::size_t> moves;
    std::size_t current_move;
    float move_duration;

    void reposition_containers();

    void move_to_end(std::size_t m, float duration);
    void push_containers_back(std::size_t m, float duration);

    void animate_move(std::size_t m, float duration = 1000.0f);
    void step_finished();
public:
    CMYKRobot();

    QRectF boundingRect() const override;

    void setup_containers(const std::string& colors);
    const std::string& get_initial_containers() const { return initial_containers_state; }
    void animate_moves(const std::vector<std::size_t>& moves, float duration = 1000.0f);

    QPointF margin() const noexcept { return margin_size; }
    QPointF containerSize() const noexcept { return container_size; }

    void setMargin(const QPointF& size);
    void setContainerSize(const QPointF& size);

    void scale_by(qreal factor);
    void update_animation_duration(double duration) { move_duration = duration; }

    void play();
    void pause();
    void resume();
    void reset();

signals:
    void animationFinished();
    void animationStateChanged(QSequentialAnimationGroup::State state);
    void animationReady();
};

#endif // ROBOT_H
