#ifndef JAMTEMPLATE_COIN_HPP
#define JAMTEMPLATE_COIN_HPP

#include <game_object.hpp>
#include <shape.hpp>
#include <vector.hpp>

class Coin : public jt::GameObject {
public:
    explicit Coin(jt::Vector2f const& pos);

    jt::Vector2f getPosition() const;

    bool canBePickedUp() const;

    std::shared_ptr<jt::DrawableInterface> getDrawable();

    void pickUp();

private:
    jt::Vector2f m_pos;
    bool m_hasBeenPickedUp { false };

    std::shared_ptr<jt::DrawableInterface> m_drawable { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_COIN_HPP
