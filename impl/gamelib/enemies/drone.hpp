#ifndef JAMTEMPLATE_BEE_HPP
#define JAMTEMPLATE_BEE_HPP

#include <animation.hpp>
#include <enemies/movement/enemy_movement_interface.hpp>
#include <box2dwrapper/box2d_object.hpp>
#include <game_object.hpp>

class Drone : public jt::GameObject {
public:
    Drone(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& position,
        std::shared_ptr<EnemyMovementInterface> movement);

private:
    std::shared_ptr<jt::Animation> m_animation { nullptr };
    std::shared_ptr<jt::Box2DObject> m_physicsObject { nullptr };

    jt::Vector2f m_position { 0.0f, 0.0f };
    std::shared_ptr<EnemyMovementInterface> m_movement { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_BEE_HPP
