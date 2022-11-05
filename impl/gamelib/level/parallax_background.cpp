#include "parallax_background.hpp"
#include <game_interface.hpp>

ParallaxBackground::ParallaxBackground(std::string era)
    : m_era(era)
{
}

void ParallaxBackground::doCreate()
{
    if (m_era == "cp" || m_era == "pa") {
        m_layer1 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_1.png", getGame()->gfx().textureManager());
        m_layer2 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_2.png", getGame()->gfx().textureManager());
        m_layer3 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_3.png", getGame()->gfx().textureManager());
        m_layer4 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_4.png", getGame()->gfx().textureManager());
        m_layer5 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_5.png", getGame()->gfx().textureManager());

        m_layer1->setCamMovementFactor(0.0f);
        m_layer2->setCamMovementFactor(0.2f);
        m_layer3->setCamMovementFactor(0.5f);
        m_layer4->setCamMovementFactor(0.8f);
        m_layer5->setCamMovementFactor(0.8f);
    }
}

void ParallaxBackground::doUpdate(float const elapsed)
{
    m_layer1->update(elapsed);
    m_layer2->update(elapsed);
    m_layer3->update(elapsed);
    m_layer4->update(elapsed);
    m_layer5->update(elapsed);
}

void ParallaxBackground::doDraw() const
{
    m_layer1->draw(renderTarget());
    m_layer2->draw(renderTarget());
    m_layer3->draw(renderTarget());
    m_layer4->draw(renderTarget());
    m_layer5->draw(renderTarget());
}
