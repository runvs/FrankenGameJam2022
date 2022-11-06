#include "coin.hpp"
#include <animation.hpp>
#include <game_interface.hpp>
#include <random/random.hpp>

Coin::Coin(jt::Vector2f const& pos)
    : m_pos { pos + jt::Vector2f { 4.0f, 4.0f } }
{
}

void Coin::doCreate()
{
    auto anim = std::make_shared<jt::Animation>();
    anim->add("assets/level/coin.png", "idle", jt::Vector2u { 8u, 8u }, { 0, 1, 2, 3 },
        jt::Random::getFloatGauss(0.2f, 0.01f), textureManager());
    anim->play("idle", jt::Random::getInt(0, 3));
    anim->setOrigin(jt::OriginMode::CENTER);

    m_drawable = anim;
    m_drawable->setColor(jt::colors::Yellow);

    m_drawable->setPosition(m_pos);
}

void Coin::doUpdate(float const elapsed) { m_drawable->update(elapsed); }

void Coin::doDraw() const { m_drawable->draw(renderTarget()); }
jt::Vector2f Coin::getPosition() const { return m_drawable->getPosition(); }
bool Coin::canBePickedUp() const { return !m_hasBeenPickedUp; }
std::shared_ptr<jt::DrawableInterface> Coin::getDrawable() { return m_drawable; }
void Coin::pickUp() { m_hasBeenPickedUp = true; }
