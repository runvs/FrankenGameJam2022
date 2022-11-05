#include "obstacle_tree.hpp"
#include "math_helper.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>

ObstacleTree::ObstacleTree(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos)
    : m_pos { pos }
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_staticBody;
    m_physicsObject = std::make_shared<jt::Box2DObject>(world, &bodyDef);
}

void ObstacleTree::doCreate()
{
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeRect(jt::Vector2f { 16.0f, 16.0f }, textureManager());
    m_shape->setPosition(m_pos);
    m_shape->setOffset(jt::OffsetMode::CENTER);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    b2PolygonShape collider {};
    collider.SetAsBox(16.0f / 2.0f, 16.0f / 2.0f);
    fixtureDef.shape = &collider;
    m_physicsObject->getB2Body()->CreateFixture(&fixtureDef);
    m_physicsObject->setPosition(m_pos);
}

void ObstacleTree::doUpdate(float const elapsed) { m_shape->update(elapsed); }

void ObstacleTree::doDraw() const
{
    if (GP::getPersistentValue("seed") != 0) {
        m_shape->draw(renderTarget());
    }
}
// TODO: Contact seedbed and change it
// TODO: When seedbed is changed, remove tree obstacle