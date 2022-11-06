#include "keycard.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>

Keycard::Keycard(jt::Vector2f const& pos, std::string const& name)
    : m_pos { pos }
    , m_name { name }
{
}

void Keycard::checkIfPlayerIsOver(jt::Vector2f const& playerPosition)
{
    if (GP::getPersistentValue(m_name) == 0) {
        jt::Rectf const rect { m_pos.x, m_pos.y, 8.0f, 8.0f };
        if (jt::MathHelper::checkIsIn(rect, playerPosition)) {
            getGame()->logger().info(
                "Player picks up keycard'" + m_name + "'", { "demo", "story_objects" });
            GP::setPersistentValue(m_name, 1);
            kill();
        }
    }
}
void Keycard::doCreate()
{
    m_drawable = std::make_shared<jt::Sprite>(
        "assets/level/tileset_cyberpunk.png", jt::Recti { 88, 8, 8, 8 }, textureManager());
}
void Keycard::doUpdate(float const elapsed) { }
void Keycard::doDraw() const
{
    if (GP::getPersistentValue(m_name) == 0) {
        m_drawable->setPosition(m_pos - jt::Vector2f { 0.0f, 0.0f });
        m_drawable->update(0.0f);
        m_drawable->draw(renderTarget());
    }
}
