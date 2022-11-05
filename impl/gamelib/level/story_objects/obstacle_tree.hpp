#ifndef OBSTACLE_TREE_HPP
#define OBSTACLE_TREE_HPP

#include "shape.hpp"
#include <box2dwrapper/box2d_object.hpp>
#include <game_object.hpp>

class ObstacleTree : public jt::GameObject {
public:
    explicit ObstacleTree(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos);

private:
    std::shared_ptr<jt::Box2DObject> m_physicsObject;

    jt::Vector2f m_pos { 0.0f, 0.0f };
    std::shared_ptr<jt::Shape> m_shape;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // OBSTACLE_TREE_HPP
