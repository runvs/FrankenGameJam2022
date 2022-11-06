#include "ginkgo_seed.hpp"
#include "math_helper.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>

GinkgoSeed::GinkgoSeed(jt::Vector2f const& pos)
    : m_pos { pos }
{
}

void GinkgoSeed::doCreate()
{
    m_drawable = std::make_shared<jt::Sprite>(
        "assets/setzling.png", jt::Recti { 0, 0, 13, 12 }, textureManager());
}

void GinkgoSeed::doUpdate(float const elapsed) { }

void GinkgoSeed::doDraw() const
{
    if (GP::getPersistentValue("ginkgo_seed") == 0) {
        m_drawable->setPosition(m_pos - jt::Vector2f { 0.0f, 6.0f });
        m_drawable->update(0.0f);
        m_drawable->draw(renderTarget());
    }
}

void GinkgoSeed::checkIfPlayerIsOver(jt::Vector2f const& playerPosition)
{
    if (GP::getPersistentValue("ginkgo_seed") == 0) {
        jt::Rectf const rect { m_pos.x, m_pos.y, 13.0f, 12.0f };
        if (jt::MathHelper::checkIsIn(rect, playerPosition)) {
            getGame()->logger().info("Player picks up ginkgo seed", { "demo", "story_objects" });
            GP::setPersistentValue("ginkgo_seed", 1);
            kill();
        }
    }
}
