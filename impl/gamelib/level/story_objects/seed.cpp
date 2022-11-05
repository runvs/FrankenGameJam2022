#include "seed.hpp"
#include "math_helper.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>

Seed::Seed(jt::Vector2f const& pos)
    : m_pos { pos }
{
}

void Seed::doCreate()
{
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeRect(jt::Vector2f { 8.0f, 8.0f }, textureManager());
    m_shape->setPosition(m_pos);
}

void Seed::doUpdate(float const elapsed) { m_shape->update(elapsed); }

void Seed::doDraw() const
{
    if (GP::getPersistentValue("seed") == 0) {
        m_shape->draw(renderTarget());
    }
}

void Seed::checkIfPlayerIsOver(jt::Vector2f const& playerPosition)
{
    if (GP::getPersistentValue("seed") == 0) {
        jt::Rectf const rect { m_pos.x, m_pos.y, 8.0f, 8.0f };
        if (jt::MathHelper::checkIsIn(rect, playerPosition)) {
            getGame()->logger().info("Player picks up seed", { "demo", "story_objects" });
            GP::setPersistentValue("seed", 1);
            kill();
        }
    }
}
