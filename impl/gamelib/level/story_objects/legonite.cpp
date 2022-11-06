
#include "legonite.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <sprite.hpp>

Legonite::Legonite(jt::Vector2f const& pos)
    : m_pos { pos }
{
}

void Legonite::doCreate()
{
    m_drawable = std::make_shared<jt::Sprite>("assets/level/legonite.png", textureManager());

    m_drawable->setPosition(m_pos + jt::Vector2f { -8.0f, -8.0f });
}
void Legonite::doUpdate(float const elapsed) { m_drawable->update(elapsed); }
void Legonite::doDraw() const { m_drawable->draw(renderTarget()); }

void Legonite::checkIfPlayerIsOver(jt::Vector2f const& playerPosition)
{
    if (GP::getPersistentValue("legonite") == 0) {
        jt::Rectf const rect { m_pos.x, m_pos.y, 13.0f, 12.0f };
        if (jt::MathHelper::checkIsIn(rect, playerPosition)) {
            getGame()->logger().info("Player picks up seed", { "demo", "story_objects" });
            GP::setPersistentValue("legonite", 1);
            kill();
        }
    }
}
