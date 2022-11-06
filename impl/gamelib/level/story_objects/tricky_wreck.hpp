#ifndef TWORLD_TRICKY_WRECK_HPP
#define TWORLD_TRICKY_WRECK_HPP

#include "box2dwrapper/box2d_object.hpp"
#include "box2dwrapper/box2d_world_interface.hpp"
#include "graphics/drawable_interface.hpp"
#include <game_object.hpp>

class TrickyWreck : public jt::GameObject {
public:
    TrickyWreck(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const& pos);

private:
    std::shared_ptr<jt::Box2DObject> m_physicsObject;

    jt::Vector2f m_pos { 0.0f, 0.0f };
    mutable std::shared_ptr<jt::DrawableInterface> m_drawable { nullptr };

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // TWORLD_TRICKY_WRECK_HPP
