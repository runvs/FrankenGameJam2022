
#include "tricky_wreck.hpp"
#include "sprite.hpp"

TrickyWreck::TrickyWreck(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos)
    : m_pos { pos }
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_staticBody;
    m_physicsObject = std::make_shared<jt::Box2DObject>(world, &bodyDef);
}

void TrickyWreck::doCreate()
{
    m_drawable = std::make_shared<jt::Sprite>("assets/level/ufowrack.png", textureManager());
    m_drawable->setPosition(m_pos);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    b2PolygonShape collider {};
    collider.SetAsBox(52.0f / 2.0f, 23.0f / 2.0f, b2Vec2 { 52.0f / 2.0f, 23.0f / 2.0f }, 0.0f);
    fixtureDef.shape = &collider;
    m_physicsObject->getB2Body()->CreateFixture(&fixtureDef);
    m_physicsObject->setPosition(m_pos);
}

void TrickyWreck::doUpdate(float const elapsed) { m_drawable->update(elapsed); }

void TrickyWreck::doDraw() const { m_drawable->draw(renderTarget()); }
