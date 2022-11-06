#include "state_game.hpp"
#include <box2dwrapper/box2d_contact_manager.hpp>
#include <box2dwrapper/box2d_world_impl.hpp>
#include <box2dwrapper/logging_box2d_contact_manager.hpp>
#include <contact_callbacks/contact_callback_player_enemy.hpp>
#include <contact_callbacks/contact_callback_player_ground.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <input/input_manager.hpp>
#include <player/platform_player.hpp>
#include <random/random.hpp>
#include <state_menu.hpp>
#include <tweens/tween_alpha.hpp>
#include <tweens/tween_position.hpp>
#include <tweens/tween_rotation.hpp>
#include <tweens/tween_scale.hpp>

StateGame::StateGame(std::string const& levelName, std::string const& targetId)
{
    m_levelName = levelName;
    m_targetId = targetId;
}

void StateGame::doInternalCreate()
{
    auto contactManager = std::make_shared<jt::Box2DContactManager>();
    auto loggingContactManager
        = std::make_shared<jt::LoggingBox2DContactManager>(contactManager, getGame()->logger());
    m_world = std::make_shared<jt::Box2DWorldImpl>(
        jt::Vector2f { 0.0f, 400.0f }, loggingContactManager);

    loadLevel();

    createPlayer();
    auto playerGroundContactListener = std::make_shared<ContactCallbackPlayerGround>();
    playerGroundContactListener->setPlayer(m_player);
    m_world->getContactManager().registerCallback("player_ground", playerGroundContactListener);

    auto playerEnemyContactListener = std::make_shared<ContactCallbackPlayerEnemy>();
    playerEnemyContactListener->setPlayer(m_player);
    m_world->getContactManager().registerCallback("player_enemy", playerEnemyContactListener);

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    setAutoDraw(false);

    if (getGame()->audio().getPermanentSound("portal") == nullptr) {
        getGame()->audio().addPermanentSound("portal", "assets/sound/portal.ogg");
    }

    if (getGame()->audio().getPermanentSound("coin_pickup") == nullptr) {
        getGame()->audio().addPermanentSound("coin_pickup", "assets/sound/player_pickup.ogg");
    }
}

void StateGame::loadLevel()
{
    m_level = std::make_shared<Level>("assets/level/" + m_levelName, m_world);

    add(m_level);

    m_coins = std::make_shared<jt::ObjectGroup<Coin>>();
    add(m_coins);
    for (auto const& p : m_level->getCoinPositions()) {
        auto c = std::make_shared<Coin>(p);
        add(c);
        m_coins->push_back(c);
    }
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (!m_ending && !getGame()->stateManager().getTransition()->isInProgress()) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());

        if (!m_player->isAlive()) {
            endGame();
        }
        m_level->checkIfPlayerIsInKillbox(m_player->getPosition(), [this]() { endGame(); });
        m_level->checkIfPlayerIsInExit(m_player->getPosition(),
            [this](std::string const& newLevelName, std::string const& newLevelTargetId) {
                if (!m_ending) {
                    getGame()->audio().getPermanentSound("portal")->play();
                    m_ending = true;
                    getGame()->stateManager().switchState(
                        std::make_shared<StateGame>(newLevelName, newLevelTargetId));
                }
            });

        m_level->checkIfPlayerIsOnStoryObject(m_player->getPosition());

        jt::Rectf playerRect { m_player->getPosition().x - 6.0f, m_player->getPosition().y - 6.0f,
            12.0f, 12.0f };
        for (auto& coin : *m_coins) {
            auto c = coin.lock();
            if (c->canBePickedUp()) {
                if (jt::MathHelper::checkIsIn(playerRect, c->getPosition())) {
                    getGame()->audio().getPermanentSound("coin_pickup")->play();

                    c->pickUp();
                    m_coinCounter++;
                    // TODO add hud display

                    auto twa = jt::TweenAlpha::create(c->getDrawable(), 0.4f, 255, 0);
                    add(twa);

                    auto tws = jt::TweenScale::create(c->getDrawable(), 0.4f,
                        jt::Vector2f { 1.0f, 1.0f }, jt::Vector2f { 3.0f, 3.0f });
                    tws->addCompleteCallback([c]() { c->kill(); });
                    add(tws);
                }
            }
        }

        handleCameraScrolling(elapsed);
    }

    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::F1)
        || getGame()->input().keyboard()->justPressed(jt::KeyCode::Escape)) {

        getGame()->stateManager().switchState(std::make_shared<StateMenu>());
    }

    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::F2)) {
        getGame()->stateManager().switchState(std::make_shared<StateGame>("level_6_pa.json"));
    }
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::F3)) {
        getGame()->stateManager().switchState(std::make_shared<StateGame>("level_6_cp.json"));
    }
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::F5)) {
        getGame()->stateManager().switchState(std::make_shared<StateGame>("level_7.json"));
    }
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::F6)) {
        getGame()->stateManager().switchState(std::make_shared<StateGame>("level_5.json"));
    }
}

void StateGame::endGame()
{
    if (!m_ending) {
        m_ending = true;
        getGame()->stateManager().switchState(std::make_shared<StateGame>(m_levelName));
    }
}

void StateGame::handleCameraScrolling(float const elapsed)
{
    auto ps = m_player->getPosOnScreen();

    float const rightMargin = 150.0f;
    float const leftMargin = 150.0f;
    float const topMargin = 75.0f;
    float const bottomMargin = 75.0f;
    float const scrollSpeed = 60.0f;
    auto& cam = getGame()->gfx().camera();

    auto const screenWidth = GP::GetScreenSize().x;
    auto const screenHeight = GP::GetScreenSize().y;

    if (ps.x < leftMargin) {
        cam.move(jt::Vector2f { -scrollSpeed * elapsed, 0.0f });
        if (ps.x < leftMargin / 2) {
            cam.move(jt::Vector2f { -scrollSpeed * elapsed, 0.0f });
        }
    } else if (ps.x > screenWidth - rightMargin) {
        cam.move(jt::Vector2f { scrollSpeed * elapsed, 0.0f });
        if (ps.x > screenWidth - rightMargin / 3 * 2) {
            cam.move(jt::Vector2f { scrollSpeed * elapsed, 0.0f });
        }
    }

    if (ps.y < topMargin) {
        cam.move(jt::Vector2f { 0.0f, -scrollSpeed * elapsed });
        if (ps.y < topMargin / 2) {
            cam.move(jt::Vector2f { 0.0f, -scrollSpeed * elapsed });
        }
    } else if (ps.y > screenHeight - bottomMargin) {
        cam.move(jt::Vector2f { 0.0f, scrollSpeed * elapsed });
        if (ps.y > screenHeight - bottomMargin / 3 * 2) {
            cam.move(jt::Vector2f { 0.0f, scrollSpeed * elapsed });
        }
    }
    clampCameraPositionToLevel();
}
void StateGame::clampCameraPositionToLevel() const
{
    auto& cam = getGame()->gfx().camera();
    // clamp camera to level bounds
    auto offset = cam.getCamOffset();
    if (offset.x < 0) {
        offset.x = 0;
    }
    if (offset.y < 0) {
        offset.y = 0;
    }
    auto const levelWidth = m_level->getLevelSizeInPixel().x;
    auto const maxCamPositionX = levelWidth - GP::GetScreenSize().x;
    if (offset.x > maxCamPositionX) {
        offset.x = maxCamPositionX;
    }
    auto const levelHeight = m_level->getLevelSizeInPixel().y;
    auto const maxCamPositionY = levelHeight - GP::GetScreenSize().y;
    if (offset.y > maxCamPositionY) {
        offset.y = maxCamPositionY;
    }
    cam.setCamOffset(offset);
}

void StateGame::doInternalDraw() const
{
    m_level->draw();

    for (auto& c : *m_coins) {
        auto coin = c.lock();
        if (coin) {
            coin->draw();
        }
    }
    m_player->draw();
    m_walkParticles->draw();
    m_playerJumpParticles->draw();
    m_vignette->draw();
}

void StateGame::createPlayer()
{
    m_player = std::make_shared<Player>(m_world);
    m_player->setPosition(m_level->getPlayerStart(m_targetId));
    m_player->setLevelSize(m_level->getLevelSizeInPixel());
    add(m_player);
    getGame()->gfx().camera().setCamOffset(m_player->getPosition() - GP::GetScreenSize() * 0.5f);
    clampCameraPositionToLevel();

    createPlayerWalkParticles();
    createPlayerJumpParticleSystem();
}

void StateGame::createPlayerJumpParticleSystem()
{
    m_playerJumpParticles = jt::ParticleSystem<jt::Shape, 50>::createPS(
        [this]() {
            auto s = std::make_shared<jt::Shape>();
            if (jt::Random::getChance()) {
                s->makeRect(jt::Vector2f { 1.0f, 1.0f }, textureManager());
            } else {
                s->makeRect(jt::Vector2f { 2.0f, 2.0f }, textureManager());
            }
            s->setColor(jt::colors::White);
            s->setPosition(jt::Vector2f { -50000, -50000 });
            s->setOrigin(jt::Vector2f { 1.0f, 1.0f });
            return s;
        },
        [this](auto s, auto p) {
            s->setPosition(p);
            s->update(0.0f);
            auto const totalTime = jt::Random::getFloat(0.2f, 0.3f);

            auto twa = jt::TweenAlpha::create(s, totalTime / 2.0f, 255, 0);
            twa->setStartDelay(totalTime / 2.0f);
            add(twa);

            auto const startPos = p;
            auto const endPos = p
                + jt::Vector2f { jt::Random::getFloatGauss(0, 4.5f),
                      jt::Random::getFloat(-2.0f, 0.0f) };
            auto twp = jt::TweenPosition::create(s, totalTime, startPos, endPos);
            add(twp);

            float minAngle = 0.0f;
            float maxAngle = 360.0f;
            if (endPos.x < startPos.x) {
                minAngle = 360.0f;
                maxAngle = 0.0f;
            }
            auto twr = jt::TweenRotation::create(s, totalTime, minAngle, maxAngle);
            add(twr);
        });
    add(m_playerJumpParticles);
    m_player->setJumpParticleSystem(m_playerJumpParticles);
}

void StateGame::createPlayerWalkParticles()
{
    m_walkParticles = jt::ParticleSystem<jt::Shape, 50>::createPS(
        [this]() {
            auto s = std::make_shared<jt::Shape>();
            s->makeRect(jt::Vector2f { 1.0f, 1.0f }, textureManager());
            s->setColor(jt::colors::Black);
            s->setPosition(jt::Vector2f { -50000, -50000 });
            return s;
        },
        [this](auto s, auto p) {
            s->setPosition(p);

            auto twa = jt::TweenAlpha::create(s, 1.5f, 255, 0);
            add(twa);

            auto const rp
                = p + jt::Vector2f { 0, 4 } + jt::Vector2f { jt::Random::getFloat(-4, 4), 0 };

            auto topPos = rp;
            auto botPos = rp;
            auto const maxHeight = jt::Random::getFloat(2.0f, 7.0f);
            auto const maxWidth = jt::Random::getFloat(2.0f, 6.0f);
            if (jt::Random::getChance()) {
                topPos = rp + jt::Vector2f { maxWidth / 2, -maxHeight };
                botPos = rp + jt::Vector2f { maxWidth, 0 };
            } else {
                topPos = rp + jt::Vector2f { -maxWidth / 2, -maxHeight };
                botPos = rp + jt::Vector2f { -maxWidth, 0 };
            }
            auto const totalTime = jt::Random::getFloat(0.3f, 0.6f);
            std::shared_ptr<jt::Tween> twp1
                = jt::TweenPosition::create(s, totalTime / 2.0f, rp, topPos);
            add(twp1);
            twp1->addCompleteCallback([this, topPos, botPos, s, totalTime]() {
                auto twp2 = jt::TweenPosition::create(s, totalTime / 2.0f, topPos, botPos);
                add(twp2);
            });
        });
    add(m_walkParticles);
    m_player->setWalkParticleSystem(m_walkParticles);
}

std::string StateGame::getName() const { return "Box2D"; }
