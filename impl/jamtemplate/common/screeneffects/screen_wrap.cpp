#include "screen_wrap.hpp"

void jt::wrapOnScreen(jt::DrawableInterface& drawable, float margin)
{
    auto const posScreen = drawable.getScreenPosition();
    auto const screenSize = drawable.getScreenSizeHint();
    auto posWorld = drawable.getPosition();

    if (screenSize.x == 0 || screenSize.y == 0) {
        return;
    }

    if (drawable.getCamMovementFactor().x != 0.0f) {
        if (posScreen.x < -margin) {
            posWorld.x += screenSize.x + margin;
        } else if (posScreen.x > screenSize.x) {
            posWorld.x -= screenSize.x + margin;
        }
    }

    if (drawable.getCamMovementFactor().y != 0.0f) {
        if (posScreen.y < -margin) {
            posWorld.y += screenSize.y + margin;
        } else if (posScreen.y > screenSize.y + margin) {
            posWorld.y -= screenSize.y + margin;
        }
    }

    drawable.setPosition(posWorld);
}
