#include "coin.hpp"
#include <game_interface.hpp>

Coin::Coin(jt::Vector2f const& pos)
    : m_pos { pos + jt::Vector2f { 4.0f, 4.0f } }
{
}

void Coin::doCreate()
{
    // TODO use real graphic sprite
    auto shape = std::make_shared<jt::Shape>();
    shape->makeRect(jt::Vector2f { 8.0f, 8.0f }, textureManager());
    shape->setOrigin(jt::OriginMode::CENTER);

    m_drawable = shape;
    m_drawable->setColor(jt::colors::Yellow);

    m_drawable->setPosition(m_pos);
}

void Coin::doUpdate(float const elapsed) { m_drawable->update(elapsed); }

void Coin::doDraw() const { m_drawable->draw(renderTarget()); }
jt::Vector2f Coin::getPosition() const { return m_drawable->getPosition(); }
bool Coin::canBePickedUp() const { return !m_hasBeenPickedUp; }
std::shared_ptr<jt::DrawableInterface> Coin::getDrawable() { return m_drawable; }
void Coin::pickUp() { m_hasBeenPickedUp = true; }
