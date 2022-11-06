
#ifndef TWORLD_LEGONITE_HPP
#define TWORLD_LEGONITE_HPP

#include <game_object.hpp>

#include <graphics/drawable_interface.hpp>

class Legonite : public jt::GameObject {
public:
    explicit Legonite(jt::Vector2f const& pos);
    void checkIfPlayerIsOver(jt::Vector2f const& playerPosition);

private:
    jt::Vector2f m_pos { 0.0f, 0.0f };
    mutable std::shared_ptr<jt::DrawableInterface> m_drawable { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // TWORLD_LEGONITE_HPP
