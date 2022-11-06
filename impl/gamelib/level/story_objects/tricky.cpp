
#include "tricky.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include <game_interface.hpp>
Tricky::Tricky(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos)
    : m_pos { pos }
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_staticBody;
    m_physicsObject = std::make_shared<jt::Box2DObject>(world, &bodyDef);
}

void Tricky::doCreate()
{
    m_want = std::make_shared<jt::Sprite>(
        "assets/tricky_want.png", jt::Recti { 0, 0, 67, 46 }, textureManager());
    m_thank = std::make_shared<jt::Sprite>(
        "assets/tricky_thank.png", jt::Recti { 0, 0, 67, 46 }, textureManager());
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    b2PolygonShape collider {};
    collider.SetAsBox(27.0f / 2.0f, 46.0f / 2.0f);
    fixtureDef.shape = &collider;
    m_physicsObject->getB2Body()->CreateFixture(&fixtureDef);
    m_physicsObject->setPosition(m_pos);
}
void Tricky::doUpdate(float const elapsed) { }
void Tricky::doDraw() const
{
    if (GP::getPersistentValue("legonite") == 0) {
        m_want->setPosition(m_pos - jt::Vector2f { 67.0f / 2.0f, 46.0f / 2.0f });
        m_want->update(0.0f);
        m_want->draw(renderTarget());
    } else {
        m_thank->setPosition(m_pos - jt::Vector2f { 67.0f / 2.0f, 46.0f / 2.0f });
        m_thank->update(0.0f);
        m_thank->draw(renderTarget());
    }
}
