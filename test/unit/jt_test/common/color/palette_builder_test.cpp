#include <color/palette_builder.hpp>
#include <gtest/gtest.h>

TEST(PaletteBuilderTest, EmptyPalette)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.create();
    ASSERT_TRUE(pal.empty());
}

TEST(PaletteBuilderTest, AddOneColor)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.addColor(jt::colors::Blue).create();
    ASSERT_EQ(pal.getColor(0), jt::colors::Blue);
}

TEST(PaletteBuilderTest, AddTwoColors)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.addColor(jt::colors::Blue).addColor(jt::colors::White).create();
    ASSERT_EQ(pal.getColor(0), jt::colors::Blue);
    ASSERT_EQ(pal.getColor(1), jt::colors::White);
}

TEST(PaletteBuilderTest, FromHEmpty)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.createGradientH(0.0f, 0.0f, 0.0f, 0.0f, 0U).create();
    ASSERT_TRUE(pal.empty());
}

TEST(PaletteBuilderTest, FromHOneEntryBlack)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.createGradientH(0.0f, 0.0f, 0.0f, 0.0f, 1U).create();
    ASSERT_EQ(pal.size(), 1U);

    auto col = pal.getColor(0);
    ASSERT_EQ(col, jt::colors::Black);
}

TEST(PaletteBuilderTest, FromHOneEntryRed)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.createGradientH(0.0f, 0.0f, 100.0f, 100.0f, 1U).create();
    ASSERT_EQ(pal.size(), 1U);

    auto col = pal.getColor(0);
    ASSERT_EQ(col, jt::colors::Red);
}

TEST(PaletteBuilderTest, FromSOneEntryBlack)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.createGradientS(0.0f, 0.0f, 0.0f, 0.0f, 1U).create();
    ASSERT_EQ(pal.size(), 1U);

    auto col = pal.getColor(0);
    ASSERT_EQ(col, jt::colors::Black);
}

TEST(PaletteBuilderTest, FromSOneEntryRed)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.createGradientS(0.0f, 100.0f, 100.0f, 100.0f, 1U).create();
    ASSERT_EQ(pal.size(), 1U);

    auto col = pal.getColor(0);
    ASSERT_EQ(col, jt::colors::Red);
}

TEST(PaletteBuilderTest, FromVOneEntryBlack)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.createGradientV(0.0f, 0.0f, 0.0f, 0.0f, 1U).create();
    ASSERT_EQ(pal.size(), 1U);

    auto col = pal.getColor(0);
    ASSERT_EQ(col, jt::colors::Black);
}

TEST(PaletteBuilderTest, FromVOneEntryRed)
{
    jt::PaletteBuilder builder;
    auto const pal = builder.createGradientV(0.0f, 100.0f, 100.0f, 100.0f, 1U).create();
    ASSERT_EQ(pal.size(), 1U);

    auto col = pal.getColor(0);
    ASSERT_EQ(col, jt::colors::Red);
}

class PaletteBuilderSizeTestFixture : public ::testing::TestWithParam<std::size_t> {
public:
    jt::PaletteBuilder builder;
};

TEST_P(PaletteBuilderSizeTestFixture, FromHSize)
{
    auto const expectedSize = GetParam();
    auto const pal = builder.createGradientH(0.0f, 0.0f, 0.0f, 0.0f, expectedSize).create();
    ASSERT_EQ(pal.size(), expectedSize);
}

TEST_P(PaletteBuilderSizeTestFixture, FromSSize)
{
    auto const expectedSize = GetParam();
    auto const pal = builder.createGradientS(0.0f, 0.0f, 0.0f, 0.0f, expectedSize).create();
    ASSERT_EQ(pal.size(), expectedSize);
}

TEST_P(PaletteBuilderSizeTestFixture, FromVSize)
{
    auto const expectedSize = GetParam();
    auto const pal = builder.createGradientV(0.0f, 0.0f, 0.0f, 0.0f, expectedSize).create();
    ASSERT_EQ(pal.size(), expectedSize);
}

INSTANTIATE_TEST_SUITE_P(
    PaletteBuilderSizeTest, PaletteBuilderSizeTestFixture, ::testing::Values(0U, 1U, 5U, 100U));

TEST(PaletteBuilderParseGPL, GPLWithOneEntry)
{
    auto const input = R"(GIMP Palette
#Palette Name: MS Paint Basic
#Description: The "Basic" Palette Used In The Program, MS Paint
#Colors: 1
64	0	64	400040

)";
    jt::PaletteBuilder builder;
    jt::Palette const pal = builder.parseGPL(input).create();
    jt::Color const expectedColor { 64, 0, 64 };
    ASSERT_EQ(pal.getColor(0), expectedColor);
}

TEST(PaletteBuilderParseGPL, GPLWithInvalidEntry)
{
    auto const input = R"(GIMP Palette
#Palette Name: MS Paint Basic
#Description: The "Basic" Palette Used In The Program, MS Paint
#Colors: 1
64	0	64	400040 1234

)";

    jt::PaletteBuilder builder;
    ASSERT_THROW((void)builder.parseGPL(input), std::invalid_argument);
}

TEST(PaletteBuilderParseGPL, GPLWithMultipleEntries)
{
    auto const input = R"(GIMP Palette
#Palette Name: MS Paint Basic
#Description: The "Basic" Palette Used In The Program
#Colors: 1
64	0	64	400040
128	255	255	80ffff
0	64	128	004080

)";

    jt::PaletteBuilder builder;
    jt::Palette const pal = builder.parseGPL(input).create();
    jt::Color const expectedColor0 { 64, 0, 64 };
    jt::Color const expectedColor1 { 128, 255, 255 };
    jt::Color const expectedColor2 { 0, 64, 128 };
    ASSERT_EQ(pal.getColor(0), expectedColor0);
    ASSERT_EQ(pal.getColor(1), expectedColor1);
    ASSERT_EQ(pal.getColor(2), expectedColor2);
}
