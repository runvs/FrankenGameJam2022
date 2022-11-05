#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
#include <memory>
#include <vector>
#include <level/level.hpp>
#include <player/platform_player.hpp>
#include <screeneffects/vignette.hpp>

// fwd decls
namespace jt {
class Shape;
class Sprite;
class Vignette;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    explicit StateGame(std::string const& levelName = "platformer_0_0.json");

private:
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    std::string m_levelName { "" };
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
};

#endif
