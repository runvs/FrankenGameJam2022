#include "exit.hpp"
#include "math_helper.hpp"
#include <game_interface.hpp>

Exit::Exit(jt::tilemap::InfoRect const& rect) { m_info = rect; }

void Exit::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>("assets/level/exit.png", textureManager());
    m_sprite->setPosition(m_info.position);

    m_glow = std::make_shared<jt::Sprite>("#g#48#150", textureManager());
    m_glow->setPosition(m_info.position - jt::Vector2f { 20.0f, 20.0f });
    m_glow->setColor(jt::Color { 255, 255, 255, 150 });
}

void Exit::doUpdate(float const elapsed)
{
    m_sprite->update(elapsed);
    m_glow->update(elapsed);
}

void Exit::doDraw() const
{
    m_glow->draw(renderTarget());
    m_sprite->draw(renderTarget());
}

bool Exit::checkIfPlayerIsInExit(jt::Vector2f const& playerPosition,
    std::function<void(std::string const&, std::string const&)> callback)
{
    jt::Rectf const exitRect { m_info.position.x, m_info.position.y, m_info.size.x, m_info.size.y };
    if (jt::MathHelper::checkIsIn(exitRect, playerPosition)) {
        auto const nextLevelName = m_info.properties.strings["next_level"];
        std::string nextLevelTargetId = "0";
        if (m_info.properties.strings.count("target_id") == 1) {
            nextLevelTargetId = m_info.properties.strings.at("target_id");
        }
        getGame()->logger().info("switch to next level: " + nextLevelName, { "platformer" });
        callback(nextLevelName, nextLevelTargetId);
        return true;
    }

    return false;
}
jt::Vector2f Exit::getPosition() const
{
    return m_sprite->getPosition() + jt::Vector2f { 5.0f, 4.0f };
}
