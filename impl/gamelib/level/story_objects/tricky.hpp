#ifndef JAMTEMPLATE_TRICKY_HPP
#define JAMTEMPLATE_TRICKY_HPP
#include "shape.hpp"
#include <box2dwrapper/box2d_object.hpp>
#include <game_object.hpp>

class Tricky : public jt::GameObject {
public:
    explicit Tricky(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos);

private:
    jt::Vector2f m_pos { 0.0f, 0.0f };
    std::shared_ptr<jt::Box2DObject> m_physicsObject;

    mutable std::shared_ptr<jt::DrawableInterface> m_want { nullptr };
    mutable std::shared_ptr<jt::DrawableInterface> m_thank { nullptr };

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_TRICKY_HPP
