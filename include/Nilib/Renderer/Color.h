#ifndef _COLOR_H
#define _COLOR_H

#include <vector>
#include <array>
#include <math.h>
#include <stdint.h>
#include <ostream>

#include "Nilib/Math/Matrix.hpp"

namespace Nilib
{

    // A humanly discernable Color.
    // Supports RGB(a), HSL, HSV,
    // AnsiConsole (Windows and Linux), TrueColor,
    // Int
    class Color
    {

        union Storage
        {
            uint8_t rgba[4];
            uint32_t val;
        } store;

    public:
        Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
        Color();

        // Get RGB(A) float values.
        void RGBAf(float *R, float *G, float *B, float *A) const;
        void RGBf(float *R, float *G, float *B) const;

        static Color RGBAf(float R, float G, float B, float A);
        static Color RGBf(float R, float G, float B);

#undef RGB // Macro in mingw32.
        static Color RGBA(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
        static Color RGB(uint8_t R, uint8_t G, uint8_t B);

        static Color HSL(float hue, float saturation, float ligh, uint8_t alpha = 255);
        static Color HSV(uint8_t hue, uint8_t saturation, uint8_t value);

        static Color RandomRGB();
        static Color RandomHSL();

        friend std::ostream &operator<<(std::ostream &os, const Color &color)
        {
            float R, G, B, A;
            color.RGBAf(&R, &G, &B, &A);
            return os << "<Color " << R << ' ' << G << ' ' << B << ' ' << A << '>';
        }

        // Convert to Vec4f.
        operator Vec4f() const
        {
            return Vec4f({store.rgba[0] / 255.0f, store.rgba[1] / 255.0f, store.rgba[2] / 255.0f, store.rgba[3] / 255.0f});
        }
    };

    namespace Colors
    {
        Color const Grey = Color::RGBf(0.411, 0.411, 0.411);
        Color const Gray = Color::RGBf(0.411, 0.411, 0.411);
        Color const Red = Color::RGBf(1.000, 0.000, 0.000);
        Color const Blue = Color::RGBf(0.000, 0.000, 1.000);
        Color const Green = Color::RGBf(0.000, 1.000, 0.000);
        Color const Yellow = Color::RGBf(1.000, 1.000, 0.000);
        Color const Orange = Color::RGBf(1.000, 0.647, 0.000);
        Color const Purple = Color::RGBf(0.502, 0.000, 0.502);
        Color const Pink = Color::RGBf(1.000, 0.753, 0.796);
        Color const Black = Color::RGBf(0.000, 0.000, 0.000);
        Color const White = Color::RGBf(1.000, 1.000, 1.000);
        Color const Brown = Color::RGBf(0.647, 0.165, 0.165);
        Color const Cyan = Color::RGBf(0.000, 1.000, 1.000);
        Color const Magenta = Color::RGBf(1.000, 0.000, 1.000);
        Color const Turquoise = Color::RGBf(0.251, 0.878, 0.816);
        Color const Indigo = Color::RGBf(0.294, 0.000, 0.510);
        Color const Maroon = Color::RGBf(0.502, 0.000, 0.000);
        Color const Olive = Color::RGBf(0.502, 0.502, 0.000);
        Color const Teal = Color::RGBf(0.000, 0.502, 0.502);
        Color const Lavender = Color::RGBf(0.902, 0.902, 0.980);
        Color const Coral = Color::RGBf(1.000, 0.498, 0.314);
        // Extended color list
        Color const Periwinkle = Color::RGBf(0.800, 0.800, 1.000);
        Color const Peach = Color::RGBf(1.000, 0.800, 0.598);
        Color const Mint = Color::RGBf(0.702, 1.000, 0.702);
        Color const Slate = Color::RGBf(0.439, 0.502, 0.565);
        Color const Charcoal = Color::RGBf(0.200, 0.200, 0.200);
        Color const Burgundy = Color::RGBf(0.500, 0.000, 0.125);
        Color const Salmon = Color::RGBf(1.000, 0.549, 0.406);
        Color const Saffron = Color::RGBf(1.000, 0.753, 0.000);
        Color const Violet = Color::RGBf(0.933, 0.510, 0.933);
        Color const Aquamarine = Color::RGBf(0.498, 1.000, 0.831);
        Color const Lime = Color::RGBf(0.000, 1.000, 0.000);
        Color const MintCream = Color::RGBf(0.960, 1.000, 0.980);
        Color const Sand = Color::RGBf(0.776, 0.658, 0.439);
        Color const Fuchsia = Color::RGBf(1.000, 0.000, 1.000);
        Color const Amber = Color::RGBf(1.000, 0.749, 0.000);
        Color const Lemon = Color::RGBf(1.000, 1.000, 0.000);
        Color const Raspberry = Color::RGBf(0.858, 0.207, 0.462);
        Color const LimeGreen = Color::RGBf(0.196, 0.804, 0.196);
        Color const SkyBlue = Color::RGBf(0.529, 0.808, 0.922);
        Color const SeaGreen = Color::RGBf(0.180, 0.545, 0.341);
        Color const Gold = Color::RGBf(1.000, 0.843, 0.000);
        Color const DarkOliveGreen = Color::RGBf(0.553, 0.352, 0.035);
        Color const ForestGreen = Color::RGBf(0.133, 0.545, 0.133);
        Color const OliveDrab = Color::RGBf(0.420, 0.557, 0.137);
        Color const LightPink = Color::RGBf(1.000, 0.713, 0.757);
        Color const AntiqueWhite = Color::RGBf(0.980, 0.922, 0.843);
        Color const Snow = Color::RGBf(1.000, 0.980, 0.980);
        Color const OldLace = Color::RGBf(0.992, 0.961, 0.902);
        Color const LavenderBlush = Color::RGBf(1.000, 0.941, 0.960);
        Color const PeachPuff = Color::RGBf(1.000, 0.855, 0.725);
        Color const MistyRose = Color::RGBf(1.000, 0.894, 0.882);
        Color const Moccasin = Color::RGBf(1.000, 0.894, 0.710);
        Color const PapayaWhip = Color::RGBf(1.000, 0.937, 0.835);
        Color const Seashell = Color::RGBf(1.000, 0.961, 0.933);
        Color const Linen = Color::RGBf(0.980, 0.941, 0.902);
        Color const LemonChiffon = Color::RGBf(1.000, 0.980, 0.804);
        Color const Cornsilk = Color::RGBf(1.000, 0.973, 0.863);
        Color const BlanchedAlmond = Color::RGBf(1.000, 0.925, 0.701);
        Color const Wheat = Color::RGBf(0.961, 0.871, 0.702);
        Color const BurlyWood = Color::RGBf(0.871, 0.722, 0.529);
        Color const Tan = Color::RGBf(0.824, 0.706, 0.549);
        Color const Peru = Color::RGBf(0.804, 0.522, 0.247);
        Color const Chocolate = Color::RGBf(0.824, 0.412, 0.118);
        Color const SaddleBrown = Color::RGBf(0.545, 0.271, 0.075);
        Color const Sienna = Color::RGBf(0.627, 0.322, 0.176);
        Color const RosyBrown = Color::RGBf(0.737, 0.560, 0.560);
        Color const IndianRed = Color::RGBf(0.804, 0.361, 0.361);
        Color const LightCoral = Color::RGBf(0.941, 0.502, 0.502);
        Color const DarkRed = Color::RGBf(0.545, 0.000, 0.000);
        Color const Firebrick = Color::RGBf(0.698, 0.132, 0.203);
        Color const DarkMagenta = Color::RGBf(0.545, 0.000, 0.545);
        Color const MediumPurple = Color::RGBf(0.576, 0.439, 0.859);
        Color const PurpleHeart = Color::RGBf(0.444, 0.259, 0.597);
        Color const Plum = Color::RGBf(0.867, 0.627, 0.867);
        Color const Orchid = Color::RGBf(0.855, 0.439, 0.839);
        Color const Thistle = Color::RGBf(0.847, 0.749, 0.847);
        Color const PaleVioletRed = Color::RGBf(0.859, 0.439, 0.576);
        Color const MediumOrchid = Color::RGBf(0.729, 0.333, 0.827);
        Color const DarkSlateBlue = Color::RGBf(0.282, 0.239, 0.545);
        Color const SlateBlue = Color::RGBf(0.416, 0.353, 0.804);
        Color const MediumSlateBlue = Color::RGBf(0.482, 0.408, 0.933);
        Color const CornflowerBlue = Color::RGBf(0.393, 0.584, 0.929);
        Color const RoyalBlue = Color::RGBf(0.255, 0.412, 0.882);
        Color const DodgerBlue = Color::RGBf(0.118, 0.565, 1.000);
        Color const LightSkyBlue = Color::RGBf(0.529, 0.808, 0.980);

        namespace Palette
        {

            template <std::size_t N>
            class Palette
            {
            public:
                Palette(std::array<Color, N> const &colors) : colors(colors) {}

                // Sample color by index (wraps around)
                inline Color const &sample(std::size_t index) const
                {
                    return colors[index % N];
                }

                inline Color const &sample() const
                {
                    return colors[Nilib::RNG::rand() % N];
                }

                inline Color const &operator[](std::size_t index) const
                {
                    return sample(index);
                }

                inline size_t size() { return N; }

            protected:
                Palette() = default;
                std::array<Color, N> colors;
            };

            template <std::size_t N>
            struct Random : public Palette<N>
            {
                Random()
                {
                    for (size_t i = 0; i < N; i++)
                        this->colors[i] = Nilib::Color::RandomHSL();
                }
            };

            Palette<3> const stoplight({Colors::Red, Colors::Orange, Colors::Green});
            Palette<3> const primary({Colors::Red, Colors::Blue, Colors::Yellow});
            Palette<6> const rainbow({Colors::Red, Colors::Orange, Colors::Yellow, Colors::Green, Colors::Blue, Colors::Purple});
            Palette<5> const cool({Colors::Teal, Colors::Cyan, Colors::SkyBlue, Colors::Blue, Colors::Indigo});
            Palette<5> const warm({Colors::Red, Colors::Orange, Colors::Coral, Colors::Gold, Colors::Amber});
            Palette<5> const earth({Colors::Brown, Colors::Olive, Colors::Tan, Colors::SaddleBrown, Colors::ForestGreen});
            Palette<5> const pastel({Colors::Lavender, Colors::PeachPuff, Colors::LightPink, Colors::MintCream, Colors::Orchid});
            Palette<5> const grayscale({Colors::Black, Colors::Charcoal, Colors::Grey, Colors::LavenderBlush, Colors::White});
            Palette<6> const vibrant({Colors::Red,
                                      Colors::Green,
                                      Colors::Blue,
                                      Colors::Magenta,
                                      Colors::Cyan,
                                      Colors::Yellow});
            Palette<5> const nature({Colors::SeaGreen, Colors::SkyBlue, Colors::Wheat, Colors::Sienna, Colors::ForestGreen});
            Palette<5> const twilight({Colors::Indigo, Colors::PurpleHeart, Colors::Orchid, Colors::Plum, Colors::SlateBlue}); // 🌅 Sunset Palette — warm, fading sky tones
            Palette<5> const sunset({Colors::Peach,
                                     Colors::Coral,
                                     Colors::Orange,
                                     Colors::Pink,
                                     Colors::Lavender});
            Palette<5> const sunrise({Colors::MistyRose,
                                      Colors::PapayaWhip,
                                      Colors::Gold,
                                      Colors::PeachPuff,
                                      Colors::LightPink});
            Palette<6> const rustic({Colors::SaddleBrown,
                                     Colors::Tan,
                                     Colors::BurlyWood,
                                     Colors::RosyBrown,
                                     Colors::Peru,
                                     Colors::Chocolate});
            Palette<6> const galaxy({Colors::SkyBlue,
                                     Colors::Indigo,
                                     Colors::SlateBlue,
                                     Colors::MediumPurple,
                                     Colors::Orchid,
                                     Colors::Thistle});
            Palette<6> const autumn({Colors::DarkRed,
                                     Colors::Sienna,
                                     Colors::Orange,
                                     Colors::OliveDrab,
                                     Colors::Tan,
                                     Colors::Wheat});
            Palette<6> const spring({Colors::LightPink,
                                     Colors::Mint,
                                     Colors::LavenderBlush,
                                     Colors::Aquamarine,
                                     Colors::SkyBlue,
                                     Colors::LemonChiffon});
            Palette<5> const winter({Colors::Slate,
                                     Colors::Charcoal,
                                     Colors::SkyBlue,
                                     Colors::MintCream,
                                     Colors::Lavender});
            Palette<5> const alertLevels({Colors::Green,
                                          Colors::Yellow,
                                          Colors::Orange,
                                          Colors::Red,
                                          Colors::DarkRed});
            Palette<7> const scientific({Colors::DarkSlateBlue,
                                         Colors::Blue,
                                         Colors::Cyan,
                                         Colors::Green,
                                         Colors::Yellow,
                                         Colors::Orange,
                                         Colors::Red});
            Palette<6> const candy({Colors::Fuchsia,
                                    Colors::Lime,
                                    Colors::Periwinkle,
                                    Colors::Peach,
                                    Colors::Mint,
                                    Colors::LightPink});
            Palette<5> const beach({Colors::Sand,
                                    Colors::Aquamarine,
                                    Colors::SkyBlue,
                                    Colors::Moccasin,
                                    Colors::Seashell});

        }; // namespace Nilib
    }; // namespace Colors

}; // namespace Nilib

#endif