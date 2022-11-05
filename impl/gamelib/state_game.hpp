#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "cam_interface.hpp"
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
#include <level/level.hpp>
#include <player/platform_player.hpp>
#include <screeneffects/vignette.hpp>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
class Vignette;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    StateGame(std::string const& levelName = "level_0.json", std::string const& targetId = "0");

private:
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    std::string m_levelName { "" };
    std::string m_targetId { "0" };
    std::shared_ptr<Level> m_level { nullptr };
    std::shared_ptr<Player> m_player { nullptr };
    std::shared_ptr<jt::Vignette> m_vignette { nullptr };

    std::shared_ptr<jt::ParticleSystem<jt::Shape, 50>> m_walkParticles { nullptr };
    std::shared_ptr<jt::ParticleSystem<jt::Shape, 50>> m_playerJumpParticles { nullptr };

    bool m_ending { false };

    std::string getName() const override;

    void doInternalCreate() override;
    void doInternalUpdate(float const /*elapsed*/) override;
    void doInternalDraw() const override;

    void createPlayer();
    void loadLevel();
    void handleCameraScrolling(float const elapsed);
    void endGame();
    void createPlayerWalkParticles();
    void createPlayerJumpParticleSystem();
    void clampCameraPositionToLevel() const;
};

#endif
