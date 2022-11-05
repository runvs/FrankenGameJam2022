#ifndef JAMTEMPLATE_SEED_BED_HPP
#define JAMTEMPLATE_SEED_BED_HPP

#include "shape.hpp"
#include <game_object.hpp>

class SeedBed : public jt::GameObject {
public:
    explicit SeedBed(jt::Vector2f const& pos);
    void checkIfPlayerIsOver(jt::Vector2f const& playerPosition);

private:
    jt::Vector2f m_pos { 0.0f, 0.0f };
    // TODO switch to real graphics
    std::shared_ptr<jt::Shape> m_shape1;
    std::shared_ptr<jt::Shape> m_shape2;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_SEED_BED_HPP
