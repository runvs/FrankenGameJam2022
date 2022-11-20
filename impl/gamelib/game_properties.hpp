#ifndef GAME_GAMEPROPERTIES_HPP
#define GAME_GAMEPROPERTIES_HPP

#include <color/color.hpp>
#include <color/palette.hpp>
#include <vector.hpp>
#include <map>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "Tworld"; }
    static std::string AuthorName()
    {
        return "Adkiem, Aneira, BloodyOrange,\n"
               "Laguna, Synchronleuchter, Thunraz";
    }
    static std::string JamName() { return "FrankenGameJam2022"; }
    static std::string JamDate() { return ""; }
    static std::string ExplanationText()
    {
        return "Win the game\n[W,A,D] to move \n[Space] to jump";
    }

    static jt::Vector2f GetWindowSize() { return jt::Vector2f { 1200, 800 }; }
    static float GetZoom() { return 3.0f; }
    static jt::Vector2f GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

    static jt::Color PaletteBackground() { return GP::getPalette().getColor(5); }
    static jt::Color PaletteFontFront() { return jt::Color { 151, 237, 239 }; }
    static jt::Color PalleteFrontHighlight() { return jt::Color { 57, 40, 122 }; }
    static jt::Color PaletteFontShadow() { return jt::Color { 22, 19, 42 }; }
    static jt::Color PaletteFontCredits() { return jt::Color { 151, 237, 239 }; }

    static jt::Palette getPalette();

    static int PhysicVelocityIterations();
    static int PhysicPositionIterations();
    static jt::Vector2f PlayerSize();

    static int getPersistentValue(std::string const& key);

    static float MusicVolume() { return 0.7f; };

    static void setPersistentValue(std::string const& key, int value);

    static void resetAllPersistentValues();

private:
    static std::map<std::string, int> m_persistentValues;
};

#endif
