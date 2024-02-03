#include "inc/gui/robot.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <functional>
#include <iterator>
#include <memory>

CMYKRobot::CMYKRobot() {
    connect(&animation_group, &QSequentialAnimationGroup::finished,
            this, &CMYKRobot::step_finished);
}

void CMYKRobot::setMargin(const QPointF& size) {
    if (margin_size != size) {
        margin_size = size;

        reposition_containers();
    }
}

void CMYKRobot::setContainerSize(const QPointF& size) {
    if (container_size != size) {
        container_size = size;

        reposition_containers();

        scene()->setSceneRect(boundingRect());
    }
}
    
void CMYKRobot::reposition_containers() {
    auto position_x = margin_size.x();
    const auto position_y = (margin_size.y() * 2) + container_size.y();
    for (const auto & ptr : containers) {
        ptr->setPos(QPointF(position_x, position_y));            
        position_x += margin_size.x() + container_size.x();
    }
}

QRectF CMYKRobot::boundingRect() const {
    const auto containers_n = containers.size();
    const auto size_x = 
        (containers_n + 4) * (container_size.x() + margin_size.x()) + margin_size.x();
    const auto size_y = 
        2 * (container_size.x() + margin_size.y()) + margin_size.y();

    return QRectF {
        0, 0,
        size_x, size_y 
    };
}

void CMYKRobot::setup_containers(const std::string& colors) {
    initial_containers_state = colors;

    for (const auto & ptr : containers)
        scene()->removeItem(ptr.get());

    containers.clear();

    auto position_x = margin_size.x();
    const auto position_y = (margin_size.y() * 2) + container_size.y();
    for (char const color : colors) {
        Container::Type type = Container::Type::None;
        switch (std::toupper(color)) {
            case 'C': type = Container::Type::C; break;
            case 'M': type = Container::Type::M; break;
            case 'Y': type = Container::Type::Y; break;
            case 'K': type = Container::Type::K; break;
            default:
                throw std::runtime_error("Invalid container type");
        } 

        const auto & ptr = *containers.insert(containers.end(), std::make_unique<Container>(
            QPointF(position_x, position_y),
            container_size,
            this
        ));

        ptr->set_type(type);

        position_x += margin_size.x() + container_size.x();
    }
}

void CMYKRobot::animate_move(std::size_t m, float duration) {
    if (m > containers.size() - 4)
        throw std::runtime_error("Invalid move");

    animation_group.clear();
    move_to_end(m, duration);
    push_containers_back(m, duration);

    const auto it = std::next(containers.begin(), m);
    std::rotate(it, std::next(it, 4), containers.end());
}
    
void CMYKRobot::animate_moves(const std::vector<std::size_t>& _moves, float duration) {
    if (animation_group.state() != QSequentialAnimationGroup::State::Stopped) {
        animation_group.stop();
        setup_containers(initial_containers_state);
    }

    moves = _moves;
    current_move = 0;
    move_duration = duration;

    if (!moves.empty()) {
        animate_move(moves[current_move++], duration);
        animationReady();
    }
}

void CMYKRobot::step_finished() {
    if (current_move < moves.size()) {
        animate_move(moves[current_move++], move_duration);
        play();
    } else {
        current_move = 0;
        animation_group.clear();
        animationFinished();    
    }
}

void CMYKRobot::move_to_end(std::size_t m, float duration) {
    constexpr auto pulling_up_down_time = 0.2f;
    
    const auto parallel_group = new QParallelAnimationGroup();
    auto it = std::next(containers.begin(), m);
    for (std::size_t i=0; i < 4; ++i) {
        const auto & ptr = *it;
        const auto start = ptr->pos();
        const auto end = QPointF {
            (containers.size() + i) * (container_size.x() + margin_size.x()) + margin_size.x(),
            start.y()
        };
        const auto pullup_point = QPointF(start.x(), margin_size.y());
        const auto pulldown_point = QPointF(end.x(), margin_size.y());

        const auto animation = new QPropertyAnimation(ptr.get(), "position");
        animation->setDuration(duration);
        animation->setStartValue(start);
        animation->setKeyValueAt(pulling_up_down_time, pullup_point);
        animation->setKeyValueAt(1.0f - pulling_up_down_time, pulldown_point);
        animation->setEndValue(end);

        parallel_group->addAnimation(animation);
        std::advance(it, 1);
    } 
    animation_group.addAnimation(parallel_group);
}

void CMYKRobot::push_containers_back(std::size_t m, float duration) {
    const auto parallel_group = new QParallelAnimationGroup();
    auto it = std::next(containers.begin(), m);

    for (std::size_t i=0; i < 4; ++i) {
        const auto & ptr = *it;
        const auto start = QPointF {
            (containers.size() + i) * (container_size.x() + margin_size.x()) + margin_size.x(),
            2 * margin_size.y() + container_size.y()
        };
        const auto end = QPointF{
            start.x() - (container_size.x() + margin_size.x()) * 4, start.y()
        };

        const auto animation = new QPropertyAnimation(ptr.get(), "position");
        animation->setStartValue(start);
        animation->setEndValue(end);
        animation->setDuration(duration);
        parallel_group->addAnimation(animation);

        std::advance(it, 1);
    }

    while (it != containers.end()) {
        const auto & ptr = *it;
        const auto start = ptr->pos();
        const auto end = QPointF {
            start.x() - (container_size.x() + margin_size.x()) * 4,
            start.y()
        };

        const auto animation = new QPropertyAnimation(ptr.get(), "position");
        animation->setStartValue(start);
        animation->setEndValue(end);
        animation->setDuration(duration);
        parallel_group->addAnimation(animation);

        std::advance(it, 1);
    }

    animation_group.addAnimation(parallel_group);
}

void CMYKRobot::play() {
    if (animation_group.state() != QSequentialAnimationGroup::State::Running) {
        animation_group.start();
        animationStateChanged(animation_group.state());
    }

}

void CMYKRobot::pause() {
    if (animation_group.state() == QSequentialAnimationGroup::State::Running) {
        animation_group.pause();
        animationStateChanged(animation_group.state());
    }
}

void CMYKRobot::resume() {
    if (animation_group.state() == QSequentialAnimationGroup::State::Paused) {
        animation_group.resume();
        animationStateChanged(animation_group.state());
    }
}

void CMYKRobot::reset() {
    if (animation_group.state() != QSequentialAnimationGroup::State::Stopped)
        animation_group.stop();
    setup_containers(initial_containers_state);
    current_move = 0;
    animate_moves(moves, move_duration);
    animationStateChanged(animation_group.state());
}

void CMYKRobot::scale_by(qreal factor) {
    QGraphicsItem::setScale(QGraphicsItem::scale() * factor); 
}
