
#include "killbox.hpp"
#include <game_interface.hpp>
#include <math_helper.hpp>
#include <sprite.hpp>

Killbox::Killbox(
    jt::Rectf const& rect, std::string const& name, std::string const& type, std::string const& era)
    : m_rect { rect }
    , m_name { name }
    , m_type { type }
    , m_era { era }
{
}

void Killbox::doCreate()
{
    std::string const tileset = m_era == "pa" ? "wasteland" : "cyberpunk";

    if (m_type == "spike_down") {
        m_drawable = std::make_shared<jt::Sprite>("assets/level/tileset_" + tileset + ".png",
            jt::Recti { 88, 16, 8, 8 }, textureManager());

    } else if (m_type == "spike_up") {
        m_drawable = std::make_shared<jt::Sprite>("assets/level/tileset_" + tileset + ".png",
            jt::Recti { 96, 16, 8, 8 }, textureManager());
    }
}
void Killbox::doUpdate(float const elapsed)
{
    if (m_drawable) {
        m_drawable->update(elapsed);
    }
}
void Killbox::doDraw() const
{
    if (m_drawable) {
        if (m_type == "spike_down" || m_type == "spike_up") {
            auto numberOfParts = static_cast<int>(m_rect.width) / 8;
            for (int i = 0; i != numberOfParts; ++i) {
                m_drawable->setPosition(jt::Vector2f { m_rect.left + i * 8.0f, m_rect.top + 0.0f });
                m_drawable->update(0.0f);
                m_drawable->draw(renderTarget());
            }
            //            m_drawable->draw(renderTarget());
        }
    }
}

void Killbox::checkIfPlayerIsInKillbox(
    jt::Vector2f const& playerPosition, std::function<void(void)> callback) const
{
    if (jt::MathHelper::checkIsIn(m_rect, playerPosition)) {
        getGame()->logger().info(
            "Player killed by killbox '" + m_name + "'", { "demo", "killbox" });
        callback();
    }
}
std::string Killbox::getName() const { return m_name; }
jt::Vector2f Killbox::getPosition() const { return jt::Vector2f { m_rect.left, m_rect.top }; }

void Killbox::setPosition(jt::Vector2f const& pos)
{
    m_rect.left = pos.x;
    m_rect.top = pos.y;
}
