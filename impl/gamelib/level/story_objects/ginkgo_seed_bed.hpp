#ifndef JAMTEMPLATE_GINKGO_SEED_BED_HPP
#define JAMTEMPLATE_GINKGO_SEED_BED_HPP

#include "shape.hpp"
#include <game_object.hpp>

class GinkgoSeedBed : public jt::GameObject {
public:
    explicit GinkgoSeedBed(jt::Vector2f const& pos);
    void checkIfPlayerIsOver(jt::Vector2f const& playerPosition);

private:
    jt::Vector2f m_pos { 0.0f, 0.0f };

    mutable std::shared_ptr<jt::DrawableInterface> m_unplanted { nullptr };
    mutable std::shared_ptr<jt::DrawableInterface> m_planted { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_GINKGO_SEED_BED_HPP
