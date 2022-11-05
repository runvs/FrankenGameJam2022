#ifndef JAMTEMPLATE_DOOR_HPP
#define JAMTEMPLATE_DOOR_HPP

#include "box2dwrapper/box2d_object.hpp"
#include "box2dwrapper/box2d_world_interface.hpp"
#include "shape.hpp"
#include <game_object.hpp>
class Door : public jt::GameObject {
public:
    explicit Door(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos);

private:
    std::shared_ptr<jt::Box2DObject> m_physicsObject;

    jt::Vector2f m_pos { 0.0f, 0.0f };
    std::shared_ptr<jt::Shape> m_shape;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};
#endif // JAMTEMPLATE_DOOR_HPP
