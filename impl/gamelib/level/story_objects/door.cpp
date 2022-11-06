
#include "door.hpp"
#include "game_properties.hpp"
#include <sprite.hpp>

Door::Door(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos)
    : m_pos { pos }
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_staticBody;
    m_physicsObject = std::make_shared<jt::Box2DObject>(world, &bodyDef);
}

void Door::doCreate()
{
    // FIXME: Shape probably not needed or something, I'm just hacking it up to be visible
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeRect(jt::Vector2f { 8.0f, 24.0f }, textureManager());
    m_shape->setPosition(m_pos);
    m_shape->setOffset(jt::OffsetMode::CENTER);

    m_drawable = std::make_shared<jt::Sprite>("assets/level/tileset_cyberpunk.png",
        jt::Recti { 88 - 16, 16 + 8, 8, 8 * 3 }, textureManager());

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    b2PolygonShape collider {};
    collider.SetAsBox(8.0f / 2.0f, 24.0f / 2.0f);
    fixtureDef.shape = &collider;
    m_physicsObject->getB2Body()->CreateFixture(&fixtureDef);
    m_physicsObject->setPosition(m_pos);
}

void Door::doUpdate(float const elapsed)
{
    m_shape->update(elapsed);
    if (GP::getPersistentValue("keycard_1") == 1 && GP::getPersistentValue("keycard_2") == 1) {
        kill();
    }
}
void Door::doDraw() const
{
    if (GP::getPersistentValue("keycard_1") == 0 || GP::getPersistentValue("keycard_2") == 0) {
        //   m_shape->draw(renderTarget());
        m_drawable->setPosition(m_pos - jt::Vector2f { 4.0f, 12.0f });
        m_drawable->update(0.0f);
        m_drawable->draw(renderTarget());
    }
}
