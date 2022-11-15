﻿#ifndef GAME_HUD_HPP
#define GAME_HUD_HPP

#include <game_object.hpp>
#include <hud/observer_interface.hpp>
#include <sprite.hpp>
#include <text.hpp>

class ScoreDisplay;

class Hud : public jt::GameObject {
public:
    std::shared_ptr<ObserverInterface<int>> getObserverScore() const;

private:
    std::shared_ptr<ScoreDisplay> m_scoreDisplay;

    jt::Text::Sptr m_scoreText;

    void doCreate() override;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;
};

#endif
