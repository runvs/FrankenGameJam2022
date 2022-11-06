
#include "door.hpp"
#include "game_properties.hpp"

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
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeRect(jt::Vector2f { 8.0f, 24.0f }, textureManager());
    m_shape->setPosition(m_pos);
    m_shape->setOffset(jt::OffsetMode::TOPLEFT);

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
        m_shape->draw(renderTarget());
    }
}
