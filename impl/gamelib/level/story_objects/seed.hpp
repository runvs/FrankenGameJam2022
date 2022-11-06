#ifndef JAMTEMPLATE_SEED_HPP
#define JAMTEMPLATE_SEED_HPP

#include "shape.hpp"
#include <game_object.hpp>

class Seed : public jt::GameObject {
public:
    explicit Seed(jt::Vector2f const& pos);
    void checkIfPlayerIsOver(jt::Vector2f const& playerPosition);

private:
    jt::Vector2f m_pos { 0.0f, 0.0f };
    mutable std::shared_ptr<jt::DrawableInterface> m_drawable { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_SEED_HPP
