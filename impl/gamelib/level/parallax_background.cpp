#include "parallax_background.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <screeneffects/screen_wrap.hpp>

ParallaxBackground::ParallaxBackground(std::string era)
    : m_era(era)
{
}

void ParallaxBackground::doCreate()
{
    if (m_era == "cp" || m_era == "pa" || m_era == "utopia") {
        m_layer1_1 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_1.png", getGame()->gfx().textureManager());
        m_layer2_1 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_2.png", getGame()->gfx().textureManager());
        m_layer3_1 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_3.png", getGame()->gfx().textureManager());
        m_layer4_1 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_4.png", getGame()->gfx().textureManager());
        m_layer5_1 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_5.png", getGame()->gfx().textureManager());

        m_layer1_1->setCamMovementFactor(jt::Vector2f { 0.0f, 0.0f });
        m_layer2_1->setCamMovementFactor(jt::Vector2f { 0.2f, 0.0f });
        m_layer3_1->setCamMovementFactor(jt::Vector2f { 0.5f, 0.0f });
        m_layer4_1->setCamMovementFactor(jt::Vector2f { 0.8f, 0.0f });
        m_layer5_1->setCamMovementFactor(jt::Vector2f { 0.8f, 0.0f });

        m_layer1_2 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_1.png", getGame()->gfx().textureManager());
        m_layer2_2 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_2.png", getGame()->gfx().textureManager());
        m_layer3_2 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_3.png", getGame()->gfx().textureManager());
        m_layer4_2 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_4.png", getGame()->gfx().textureManager());
        m_layer5_2 = std::make_shared<jt::Sprite>(
            "assets/background/" + m_era + "_5.png", getGame()->gfx().textureManager());

        m_layer1_2->setPosition(jt::Vector2f { 400.0f, 0.0f });
        m_layer2_2->setPosition(jt::Vector2f { 400.0f, 0.0f });
        m_layer3_2->setPosition(jt::Vector2f { 400.0f, 0.0f });
        m_layer4_2->setPosition(jt::Vector2f { 400.0f, 0.0f });
        m_layer5_2->setPosition(jt::Vector2f { 400.0f, 0.0f });

        m_layer1_2->setCamMovementFactor(jt::Vector2f { 0.0f, 0.0f });
        m_layer2_2->setCamMovementFactor(jt::Vector2f { 0.2f, 0.0f });
        m_layer3_2->setCamMovementFactor(jt::Vector2f { 0.5f, 0.0f });
        m_layer4_2->setCamMovementFactor(jt::Vector2f { 0.8f, 0.0f });
        m_layer5_2->setCamMovementFactor(jt::Vector2f { 0.8f, 0.0f });

        m_layer3_1->setScreenSizeHint(GP::GetScreenSize());
    }
}

void ParallaxBackground::doUpdate(float const elapsed)
{
    jt::wrapOnScreen(*m_layer2_1.get(), 400.0f);
    jt::wrapOnScreen(*m_layer3_1.get(), 400.0f);
    jt::wrapOnScreen(*m_layer4_1.get(), 400.0f);
    jt::wrapOnScreen(*m_layer5_1.get(), 400.0f);

    jt::wrapOnScreen(*m_layer2_2.get(), 400.0f);
    jt::wrapOnScreen(*m_layer3_2.get(), 400.0f);
    jt::wrapOnScreen(*m_layer4_2.get(), 400.0f);
    jt::wrapOnScreen(*m_layer5_2.get(), 400.0f);

    m_layer1_1->update(elapsed);
    m_layer2_1->update(elapsed);
    m_layer3_1->update(elapsed);
    m_layer4_1->update(elapsed);
    m_layer5_1->update(elapsed);

    m_layer1_2->update(elapsed);
    m_layer2_2->update(elapsed);
    m_layer3_2->update(elapsed);
    m_layer4_2->update(elapsed);
    m_layer5_2->update(elapsed);
}

void ParallaxBackground::doDraw() const
{
    m_layer1_1->draw(renderTarget());
    m_layer2_1->draw(renderTarget());
    m_layer3_1->draw(renderTarget());
    m_layer4_1->draw(renderTarget());
    m_layer5_1->draw(renderTarget());

    m_layer1_2->draw(renderTarget());
    m_layer2_2->draw(renderTarget());
    m_layer3_2->draw(renderTarget());
    m_layer4_2->draw(renderTarget());
    m_layer5_2->draw(renderTarget());
}
