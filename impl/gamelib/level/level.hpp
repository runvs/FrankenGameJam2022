#ifndef JAMTEMPLATE_LEVEL_HPP
#define JAMTEMPLATE_LEVEL_HPP

#include "coin.hpp"
#include "level/story_objects/door.hpp"
#include "level/story_objects/keycard.hpp"
#include "level/story_objects/seed_bed.hpp"
#include "level/story_objects/tricky.hpp"
#include "level/story_objects/tricky_wreck.hpp"
#include "parallax_background.hpp"
#include <box2dwrapper/box2d_object.hpp>
#include <box2dwrapper/box2d_world_interface.hpp>
#include <enemies/drone.hpp>
#include <game_object.hpp>
#include <level/exit.hpp>
#include <level/killbox.hpp>
#include <level/moving_platform.hpp>
#include <level/story_objects/obstacle_tree.hpp>
#include <level/story_objects/seed.hpp>
#include <object_group.hpp>
#include <shape.hpp>
#include <tilemap/tile_layer.hpp>
#include <tilemap/tileson_loader.hpp>
#include <functional>

class Level : public jt::GameObject {
public:
    Level(std::string const& fileName, std::weak_ptr<jt::Box2DWorldInterface> world);
    jt::Vector2f getPlayerStart(std::string const& id) const;

    void checkIfPlayerIsInKillbox(
        jt::Vector2f const& playerPosition, std::function<void(void)> callback) const;

    void checkIfPlayerIsInExit(jt::Vector2f const& playerPosition,
        std::function<void(std::string const&, std::string const&)> callback);

    void checkIfPlayerIsOnStoryObject(jt::Vector2f const& playerPosition);

    jt::Vector2f getLevelSizeInPixel() const;
    std::vector<jt::Vector2f> getLevelExitPositions() const;

    std::vector<jt::Vector2f> getCoinPositions() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::string m_levelEra { "" };

    std::shared_ptr<ParallaxBackground> m_background;

    //    std::shared_ptr<jt::Shape> m_background { nullptr };
    std::string m_fileName { "" };
    std::weak_ptr<jt::Box2DWorldInterface> m_world {};

    std::vector<std::shared_ptr<jt::Box2DObject>> m_colliders {};
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerGround { nullptr };
    std::map<std::string, jt::Vector2f> m_startPositions { { "0", { 0.0f, 0.0f } } };
    // TODO create enemy interface/base class
    std::vector<std::shared_ptr<Drone>> m_bees {};

    std::vector<std::shared_ptr<Killbox>> m_killboxes {};
    std::vector<Exit> m_exits {};
    jt::Vector2f m_levelSizeInPixel { 0.0f, 0.0f };

    std::vector<std::shared_ptr<MovingPlatform>> m_movingPlatforms {};

    // story objects
    std::shared_ptr<Seed> m_seed { nullptr };
    std::shared_ptr<SeedBed> m_seedbed { nullptr };
    std::shared_ptr<ObstacleTree> m_obstacle_tree { nullptr };
    std::shared_ptr<Keycard> m_keycard { nullptr };
    std::shared_ptr<Door> m_door { nullptr };

    std::shared_ptr<Tricky> m_tricky { nullptr };
    std::shared_ptr<TrickyWreck> m_wreck { nullptr };
    /* std::shared_ptr<Legonite> m_legonite { nullptr };
    std::shared_ptr<GinkgoSeed> m_ginkgo_seed { nullptr };
    std::shared_ptr<GinkgoSeedbed> m_ginkgo_seedbed { nullptr };
    */
    std::vector<jt::Vector2f> m_coinPositions {};

    void loadLevelSettings(jt::tilemap::TilesonLoader& loader);
    void loadLevelTileLayer(jt::tilemap::TilesonLoader& loader);
    void loadLevelCollisions(jt::tilemap::TilesonLoader& loader);
    void loadLevelKillboxes(jt::tilemap::TilesonLoader& loader);
    void loadLevelSize(jt::tilemap::TilesonLoader const& loader);
    void loadMovingPlatforms(jt::tilemap::TilesonLoader& loader);
    void loadEnemies(jt::tilemap::TilesonLoader& loader);

    void loadStoryObjects(jt::tilemap::TilesonLoader& loader);
    void loadCoins(jt::tilemap::TilesonLoader& loader);
};

#endif // JAMTEMPLATE_LEVEL_HPP
