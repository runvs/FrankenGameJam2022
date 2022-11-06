#include "drone.hpp"
#include <level/user_data_entries.hpp>
#include <Box2D/Box2D.h>

Drone::Drone(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& position,
    std::shared_ptr<EnemyMovementInterface> movement)
{
    if (movement == nullptr) {
        throw std::invalid_argument { "invalid movement pattern" };
    }
    m_position = position;

    m_movement = movement;

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_kinematicBody;
    m_physicsObject = std::make_shared<jt::Box2DObject>(world, &bodyDef);
}

void Drone::doCreate()
{
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    b2CircleShape circleCollider {};
    circleCollider.m_radius = 3.5f;
    circleCollider.m_p.Set(3.5f, 3.5f);
    fixtureDef.shape = &circleCollider;
    auto collider = m_physicsObject->getB2Body()->CreateFixture(&fixtureDef);
    m_physicsObject->setPosition(m_position);
    collider->SetUserData((void*)g_userDataEnemyID);

    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromJson("assets/enemies/drone.json", textureManager());
    m_animation->play("idle");
    m_animation->setPosition(jt::Vector2f { 100.0f, 100.0f });
}

void Drone::doUpdate(float const elapsed)
{
    m_movement->update(elapsed, m_physicsObject);
    m_animation->setPosition(m_physicsObject->getPosition());
    m_animation->update(elapsed);
}

void Drone::doDraw() const { m_animation->draw(renderTarget()); }
