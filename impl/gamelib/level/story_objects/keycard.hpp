#ifndef JAMTEMPLATE_KEYCARD_HPP
#define JAMTEMPLATE_KEYCARD_HPP

#include "shape.hpp"
#include <game_object.hpp>

class Keycard : public jt::GameObject {
public:
    Keycard(jt::Vector2f const& pos, std::string const& name);
    void checkIfPlayerIsOver(jt::Vector2f const& playerPosition);

private:
    jt::Vector2f m_pos { 0.0f, 0.0f };
    std::string m_name { "" };

    mutable std::shared_ptr<jt::DrawableInterface> m_drawable { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_KEYCARD_HPP
