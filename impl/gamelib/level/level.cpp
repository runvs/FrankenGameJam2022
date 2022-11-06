#include "level.hpp"
#include <enemies/movement/enemy_movement_horizontal.hpp>
#include <enemies/movement/enemy_movement_vertical.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <strutils.hpp>
#include <tilemap/tileson_loader.hpp>
#include <Box2D/Box2D.h>

Level::Level(std::string const& fileName, std::weak_ptr<jt::Box2DWorldInterface> world)
{
    m_fileName = fileName;
    m_world = world;
}

void Level::doCreate()
{
    //    m_background = std::make_shared<jt::Shape>();
    //    m_background->makeRect(GP::GetScreenSize(), textureManager());
    //
    //    m_background->setCamMovementFactor(0.0f);

    jt::tilemap::TilesonLoader loader { getGame()->cache().getTilemapCache(), m_fileName };

    loadLevelSize(loader);
    loadLevelSettings(loader);
    loadLevelTileLayer(loader);
    loadLevelCollisions(loader);
    loadLevelKillboxes(loader);
    loadMovingPlatforms(loader);
    loadEnemies(loader);
    loadCoins(loader);

    loadStoryObjects(loader);

    m_background = std::make_shared<ParallaxBackground>(m_levelEra);
    m_background->setGameInstance(getGame());
    m_background->create();
}

void Level::loadEnemies(jt::tilemap::TilesonLoader& loader)
{
    auto const enemies = loader.loadObjectsFromLayer("enemies");
    for (auto const& enemy : enemies) {
        if (enemy.name == "bee") {
            std::shared_ptr<EnemyMovementInterface> movement { nullptr };
            if (enemy.properties.strings.at("movement") == "horizontal") {
                movement = std::make_shared<EnemyMovementHorizontal>(enemy.position.x,
                    enemy.position.x + enemy.properties.ints.at("distance_in_tiles") * 8.0f);
            } else if (enemy.properties.strings.at("movement") == "vertical") {
                movement = std::make_shared<EnemyMovementVertical>(enemy.position.y,
                    enemy.position.y + enemy.properties.ints.at("distance_in_tiles") * 8.0f);
            }
            auto bee = std::make_shared<Drone>(m_world.lock(), enemy.position, movement);
            bee->setGameInstance(getGame());
            bee->create();
            m_bees.push_back(bee);
        }
    }
}

void Level::loadMovingPlatforms(jt::tilemap::TilesonLoader& loader)
{
    auto const platform_infos = loader.loadObjectsFromLayer("platforms");
    std::map<std::string, std::pair<jt::Vector2f, float>> allPositionsInLevel;
    for (auto const& p : platform_infos) {
        if (p.properties.strings.count("type") == 0) {
            float waitTime = 0.0f;
            if (p.properties.floats.count("wait")) {
                waitTime = p.properties.floats.at("wait");
            }
            allPositionsInLevel[p.name] = std::make_pair(p.position, waitTime);
        }
    }
    for (auto const& p : platform_infos) {
        if (!p.properties.strings.empty()) {
            std::vector<std::pair<jt::Vector2f, float>> currentPlatformPositions;
            auto const positionsString = p.properties.strings.at("positions");
            auto const individualPositionStrings = strutil::split(positionsString, ",");
            for (auto const& ps : individualPositionStrings) {
                if (allPositionsInLevel.count(ps) == 0) {
                    getGame()->logger().warning("position not found in level: " + ps, { "level" });
                }
                currentPlatformPositions.push_back(allPositionsInLevel[ps]);
            }
            float timeoffset = 0.0f;
            if (p.properties.floats.count("timeoffset") == 1) {
                timeoffset = p.properties.floats.at("timeoffset");
            }
            auto platform
                = std::make_shared<MovingPlatform>(m_world.lock(), p.size, currentPlatformPositions,
                    p.properties.floats.at("velocity"), timeoffset, m_levelEra);
            std::string linkedKillbox { "" };
            if (p.properties.strings.count("linked_killbox") == 1) {
                linkedKillbox = p.properties.strings.at("linked_killbox");
            }
            if (!linkedKillbox.empty()) {
                std::shared_ptr<Killbox> ptr { nullptr };
                for (auto kb : m_killboxes) {
                    if (kb->getName() == linkedKillbox) {
                        ptr = kb;
                        break;
                    }
                }

                if (ptr != nullptr) {
                    platform->setLinkedKillbox(ptr);
                }
            }
            platform->setGameInstance(getGame());
            platform->create();
            m_movingPlatforms.push_back(platform);
        }
    }
}

void Level::loadLevelSize(jt::tilemap::TilesonLoader const& loader)
{
    auto const sizeInTiles = loader.getMapSizeInTiles();
    m_levelSizeInPixel = jt::Vector2f { 8.0f * sizeInTiles.x, 8.0f * sizeInTiles.y };
}

void Level::loadLevelKillboxes(jt::tilemap::TilesonLoader& loader)
{
    auto killboxInfos = loader.loadObjectsFromLayer("killboxes");
    for (auto const& i : killboxInfos) {
        std::string name { i.name };
        std::string type { "" };
        if (i.properties.strings.count("type") == 1) {
            type = i.properties.strings.at("type");
        }
        auto kb = std::make_shared<Killbox>(
            jt::Rectf { i.position.x, i.position.y, i.size.x, i.size.y }, name, type, m_levelEra);
        kb->setGameInstance(getGame());

        kb->create();
        m_killboxes.push_back(kb);
    }
}

void Level::loadLevelCollisions(jt::tilemap::TilesonLoader& loader)
{
    auto tileCollisions = loader.loadCollisionsFromLayer("ground");

    tileCollisions.refineColliders(8);
    for (auto const& r : tileCollisions.getRects()) {
        b2BodyDef bodyDef;
        bodyDef.fixedRotation = true;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(r.left + r.width / 2.0f, r.top + r.height / 2.0f);

        b2FixtureDef fixtureDef;
        b2PolygonShape boxCollider {};
        boxCollider.SetAsBox(r.width / 2.0f, r.height / 2.0f);
        fixtureDef.shape = &boxCollider;

        auto collider = std::make_shared<jt::Box2DObject>(m_world.lock(), &bodyDef);
        collider->getB2Body()->CreateFixture(&fixtureDef);

        m_colliders.push_back(collider);
    }
}

void Level::loadLevelTileLayer(jt::tilemap::TilesonLoader& loader)
{
    m_tileLayerGround = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("ground", textureManager(), "assets/level/"));
}

void Level::loadLevelSettings(jt::tilemap::TilesonLoader& loader)
{
    auto settings = loader.loadObjectsFromLayer("settings");
    for (auto const& info : settings) {

        if (info.name == "map_settings") {
            m_levelEra = "cp";
            if (info.properties.strings.count("era") == 1) {
                m_levelEra = info.properties.strings.at("era");
            }
        } else if (info.name == "player_start") {
            std::string id = "0";
            if (info.properties.strings.count("id") == 1) {
                id = info.properties.strings.at("id");
            }
            m_startPositions[id] = info.position;

        } else if (info.name == "exit") {
            auto exit = Exit { info };
            exit.setGameInstance(getGame());
            exit.create();
            m_exits.emplace_back(exit);
        }
    }
}

void Level::loadStoryObjects(jt::tilemap::TilesonLoader& loader)
{
    auto storyObjects = loader.loadObjectsFromLayer("story_objects");

    for (auto const& sr : storyObjects) {
        if (sr.name == "seed") {
            if (GP::getPersistentValue("seed") == 0) {
                getGame()->logger().info("seed created", { "story_objects" });
                m_seed = std::make_shared<Seed>(sr.position);
                m_seed->setGameInstance(getGame());
                m_seed->create();
            }

        } else if (sr.name == "seedbed") {
            if (GP::getPersistentValue("seedbed") == 0) {
                getGame()->logger().info("seedbed created", { "story_objects" });
                m_seedbed = std::make_shared<SeedBed>(sr.position);
                m_seedbed->setGameInstance(getGame());
                m_seedbed->create();
            }
        } else if (sr.name == "obstacle_tree") {
            if (GP::getPersistentValue("seed") != 0) {
                getGame()->logger().info("tree created", { "story_objects" });
                m_obstacle_tree = std::make_shared<ObstacleTree>(m_world.lock(), sr.position);
                m_obstacle_tree->setGameInstance(getGame());
                m_obstacle_tree->create();
            }
        } else if (strutil::starts_with(sr.name, "keycard")) {
            if (GP::getPersistentValue(sr.name) == 0) {
                getGame()->logger().info("'" + sr.name + "' created", { "story_objects" });
                m_keycard = std::make_shared<Keycard>(sr.position, sr.name);
                m_keycard->setGameInstance(getGame());
                m_keycard->create();
            }
        } else if (sr.name == "door") {
            if (GP::getPersistentValue("keycard_1") != 1
                || GP::getPersistentValue("keycard_2") != 1) {
                getGame()->logger().info("door created", { "story_objects" });
                m_door = std::make_shared<Door>(m_world.lock(), sr.position);
                m_door->setGameInstance(getGame());
                m_door->create();
            }
        } // TODO: "tricky", "legonite"
        // TODO: gingko seed?
    }
}

void Level::doUpdate(float const elapsed)
{
    m_background->update(elapsed);
    m_tileLayerGround->update(elapsed);
    for (auto& exit : m_exits) {
        exit.update(elapsed);
    }
    for (auto& p : m_movingPlatforms) {
        p->update(elapsed);
    }
    for (auto& b : m_bees) {
        b->update(elapsed);
    }
    for (auto& kb : m_killboxes) {
        kb->update(elapsed);
    }

    if (m_seed) {
        m_seed->update(elapsed);
        if (!m_seed->isAlive()) {
            m_seed.reset();
        }
    }

    if (m_seedbed) {
        m_seedbed->update(elapsed);
    }

    if (m_obstacle_tree) {
        m_obstacle_tree->update(elapsed);
    }
    if (m_keycard) {
        m_keycard->update(elapsed);
        if (!m_keycard->isAlive()) {
            m_keycard.reset();
        }
    }
    if (m_door) {
        m_door->update(elapsed);
        if (!m_door->isAlive()) {
            m_door.reset();
        }
    }
}

void Level::doDraw() const
{
    m_background->draw();
    m_tileLayerGround->draw(renderTarget());
    for (auto const& exit : m_exits) {
        exit.draw();
    }
    for (auto const& p : m_movingPlatforms) {
        p->draw();
    }
    for (auto const& b : m_bees) {
        b->draw();
    }
    for (auto const& kb : m_killboxes) {
        kb->draw();
    }
    if (m_seed) {
        m_seed->draw();
    }
    if (m_seedbed) {
        m_seedbed->draw();
    }
    if (m_obstacle_tree) {
        m_obstacle_tree->draw();
    }
    if (m_keycard) {
        m_keycard->draw();
    }
    if (m_door) {
        m_door->draw();
    }
}

jt::Vector2f Level::getPlayerStart(std::string const& id) const { return m_startPositions.at(id); }

void Level::checkIfPlayerIsInKillbox(
    jt::Vector2f const& playerPosition, std::function<void(void)> callback) const
{
    for (auto const& kb : m_killboxes) {
        kb->checkIfPlayerIsInKillbox(playerPosition, callback);
    }
}

void Level::checkIfPlayerIsInExit(jt::Vector2f const& playerPosition,
    std::function<void(std::string const&, std::string const&)> callback)
{
    for (auto& exit : m_exits) {
        if (exit.checkIfPlayerIsInExit(playerPosition, callback)) {
            break;
        }
    }
}

jt::Vector2f Level::getLevelSizeInPixel() const { return m_levelSizeInPixel; }

void Level::checkIfPlayerIsOnStoryObject(jt::Vector2f const& playerPosition)
{
    if (m_seed) {
        m_seed->checkIfPlayerIsOver(playerPosition);
    }
    if (m_seedbed) {
        m_seedbed->checkIfPlayerIsOver(playerPosition);
    }
    if (m_keycard) {

        m_keycard->checkIfPlayerIsOver(playerPosition);
    }
}

std::vector<jt::Vector2f> Level::getCoinPositions() const { return m_coinPositions; }

void Level::loadCoins(jt::tilemap::TilesonLoader& loader)
{
    auto coins = loader.loadObjectsFromLayer("coins");

    for (auto& c : coins) {
        m_coinPositions.push_back(c.position);
    }
}
std::vector<jt::Vector2f> Level::getLevelExitPositions() const
{
    std::vector<jt::Vector2f> positions;
    for (auto& e : m_exits) {
        positions.push_back(e.getPosition());
    }

    return positions;
}
