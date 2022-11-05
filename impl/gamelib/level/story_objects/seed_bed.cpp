
#include "seed_bed.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include <game_interface.hpp>
SeedBed::SeedBed(jt::Vector2f const& pos)
    : m_pos { pos }
{
}

void SeedBed::checkIfPlayerIsOver(jt::Vector2f const& playerPosition)
{
    jt::Rectf const rect { m_pos.x, m_pos.y, 8.0f, 8.0f };
    if (jt::MathHelper::checkIsIn(rect, playerPosition)) {
        getGame()->logger().info("Player walks over seedbed", { "story_objects" });
        if (GP::getPersistentValue("seed") == 1 && GP::getPersistentValue("seedbed") == 0) {
            getGame()->logger().info("Player places seed", { "story_objects" });
            GP::setPersistentValue("seedbed", 1);
        }
    }
}

void SeedBed::doCreate()
{
    // TODO load real graphics
    m_shape1 = std::make_shared<jt::Shape>();
    m_shape1->makeRect(jt::Vector2f { 8.0f, 8.0f }, textureManager());
    m_shape1->setPosition(m_pos);
    m_shape1->setColor(jt::colors::Yellow);

    m_shape2 = std::make_shared<jt::Shape>();
    m_shape2->makeRect(jt::Vector2f { 8.0f, 8.0f }, textureManager());
    m_shape2->setPosition(m_pos);
    m_shape2->setColor(jt::colors::Green);
}
void SeedBed::doUpdate(float const elapsed)
{
    m_shape1->update(elapsed);
    m_shape2->update(elapsed);
}
void SeedBed::doDraw() const
{
    if (GP::getPersistentValue("seedbed") == 0) {
        m_shape1->draw(renderTarget());
    } else {
        m_shape2->draw(renderTarget());
    }
}
