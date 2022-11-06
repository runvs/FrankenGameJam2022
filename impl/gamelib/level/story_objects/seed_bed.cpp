
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
    m_unplanted = std::make_shared<jt::Sprite>(
        "assets/level/tileset_cyberpunk.png", jt::Recti { 0, 32, 32, 8 }, textureManager());
    m_planted = std::make_shared<jt::Sprite>(
        "assets/seeded_tree.png", jt::Recti { 0, 0, 32, 20 }, textureManager());
}
void SeedBed::doUpdate(float const elapsed) { }
void SeedBed::doDraw() const
{
    if (GP::getPersistentValue("seedbed") == 0) {
        m_unplanted->setPosition(m_pos - jt::Vector2f { 0.0f, 0.0f });
        m_unplanted->update(0.0f);
        m_unplanted->draw(renderTarget());
    } else {
        m_planted->setPosition(m_pos - jt::Vector2f { 0.0f, 12.0f });
        m_planted->update(0.0f);
        m_planted->draw(renderTarget());
    }
}
