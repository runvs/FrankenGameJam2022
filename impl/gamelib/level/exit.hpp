#ifndef JAMTEMPLATE_EXIT_HPP
#define JAMTEMPLATE_EXIT_HPP

#include <game_object.hpp>
#include <sprite.hpp>
#include <tilemap/info_rect.hpp>
#include <functional>
#include <memory>

class Exit : public jt::GameObject {
public:
    explicit Exit(jt::tilemap::InfoRect const& rect);

    bool checkIfPlayerIsInExit(jt::Vector2f const& playerPosition,
        std::function<void(std::string const&, std::string const&)> callback);

    jt::Vector2f getPosition() const;

private:
    jt::tilemap::InfoRect m_info {};
    std::shared_ptr<jt::Sprite> m_sprite { nullptr };
    std::shared_ptr<jt::Sprite> m_glow { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_EXIT_HPP
