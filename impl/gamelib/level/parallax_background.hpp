#ifndef JAMTEMPLATE_PARALLAX_BACKGROUND_HPP
#define JAMTEMPLATE_PARALLAX_BACKGROUND_HPP

#include <game_object.hpp>
#include <sprite.hpp>

class ParallaxBackground : public jt::GameObject {
public:
    explicit ParallaxBackground(std::string era);

private:
    std::string m_era { "" };

    std::shared_ptr<jt::Sprite> m_layer1 { nullptr };
    std::shared_ptr<jt::Sprite> m_layer2 { nullptr };
    std::shared_ptr<jt::Sprite> m_layer3 { nullptr };
    std::shared_ptr<jt::Sprite> m_layer4 { nullptr };
    std::shared_ptr<jt::Sprite> m_layer5 { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_PARALLAX_BACKGROUND_HPP
