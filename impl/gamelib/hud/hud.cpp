#include "hud.hpp"
#include <color/color.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <hud/score_display.hpp>

std::shared_ptr<ObserverInterface<int>> Hud::getObserverScore() const { return m_scoreDisplay; }

void Hud::doCreate()
{
    m_scoreText = std::make_shared<jt::Text>();
    m_scoreText = jt::dh::createText(renderTarget(), "", 16, jt::Color { 248, 249, 254 });
    m_scoreText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_scoreText->setPosition({ 10, 4 });
    m_scoreText->setIgnoreCamMovement(true);

    m_scoreDisplay = std::make_shared<ScoreDisplay>(m_scoreText, "Coins: ");
}

void Hud::doUpdate(float const elapsed) { m_scoreText->update(elapsed); }

void Hud::doDraw() const { m_scoreText->draw(renderTarget()); }
