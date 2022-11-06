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
    m_drawable = std::make_shared<jt::Sprite>(
        "assets/baum.png", jt::Recti { 0, 0, 16, 16 }, textureManager());
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    b2PolygonShape collider {};
    collider.SetAsBox(16.0f / 2.0f, 16.0f / 2.0f);
    fixtureDef.shape = &collider;
    m_physicsObject->getB2Body()->CreateFixture(&fixtureDef);
    m_physicsObject->setPosition(m_pos);
}

void ObstacleTree::doUpdate(float const elapsed) { }

void ObstacleTree::doDraw() const
{
    if (GP::getPersistentValue("seed") != 0) {
        m_drawable->setPosition(m_pos - jt::Vector2f { 8.0f, 8.0f });
        m_drawable->update(0.0f);
        m_drawable->draw(renderTarget());
    }
}