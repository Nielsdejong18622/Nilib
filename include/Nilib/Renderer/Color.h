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
        Color const Grey = Color::RGBf(0.411f, 0.411f, 0.411f);
        Color const Gray = Color::RGBf(0.411f, 0.411f, 0.411f);
        Color const Red = Color::RGBf(1.000f, 0.000f, 0.000f);
        Color const Blue = Color::RGBf(0.000f, 0.000f, 1.000f);
        Color const Green = Color::RGBf(0.000f, 1.000f, 0.000f);
        Color const Yellow = Color::RGBf(1.000f, 1.000f, 0.000f);
        Color const Orange = Color::RGBf(1.000f, 0.647f, 0.000f);
        Color const Purple = Color::RGBf(0.502f, 0.000f, 0.502f);
        Color const Pink = Color::RGBf(1.000f, 0.753f, 0.796f);
        Color const Black = Color::RGBf(0.000f, 0.000f, 0.000f);
        Color const White = Color::RGBf(1.000f, 1.000f, 1.000f);
        Color const Brown = Color::RGBf(0.647f, 0.165f, 0.165f);
        Color const Cyan = Color::RGBf(0.000f, 1.000f, 1.000f);
        Color const Magenta = Color::RGBf(1.000f, 0.000f, 1.000f);
        Color const Turquoise = Color::RGBf(0.251f, 0.878f, 0.816f);
        Color const Indigo = Color::RGBf(0.294f, 0.000f, 0.510f);
        Color const Maroon = Color::RGBf(0.502f, 0.000f, 0.000f);
        Color const Olive = Color::RGBf(0.502f, 0.502f, 0.000f);
        Color const Teal = Color::RGBf(0.000f, 0.502f, 0.502f);
        Color const Lavender = Color::RGBf(0.902f, 0.902f, 0.980f);
        Color const Coral = Color::RGBf(1.000f, 0.498f, 0.314f);
        // Extended color list
        Color const Periwinkle = Color::RGBf(0.800f, 0.800f, 1.000f);
        Color const Peach = Color::RGBf(1.000f, 0.800f, 0.598f);
        Color const Mint = Color::RGBf(0.702f, 1.000f, 0.702f);
        Color const Slate = Color::RGBf(0.439f, 0.502f, 0.565f);
        Color const Charcoal = Color::RGBf(0.200f, 0.200f, 0.200f);
        Color const Burgundy = Color::RGBf(0.500f, 0.000f, 0.125f);
        Color const Salmon = Color::RGBf(1.000f, 0.549f, 0.406f);
        Color const Saffron = Color::RGBf(1.000f, 0.753f, 0.000f);
        Color const Violet = Color::RGBf(0.933f, 0.510f, 0.933f);
        Color const Aquamarine = Color::RGBf(0.498f, 1.000f, 0.831f);
        Color const Lime = Color::RGBf(0.000f, 1.000f, 0.000f);
        Color const MintCream = Color::RGBf(0.960f, 1.000f, 0.980f);
        Color const Sand = Color::RGBf(0.776f, 0.658f, 0.439f);
        Color const Fuchsia = Color::RGBf(1.000f, 0.000f, 1.000f);
        Color const Amber = Color::RGBf(1.000f, 0.749f, 0.000f);
        Color const Lemon = Color::RGBf(1.000f, 1.000f, 0.000f);
        Color const Raspberry = Color::RGBf(0.858f, 0.207f, 0.462f);
        Color const LimeGreen = Color::RGBf(0.196f, 0.804f, 0.196f);
        Color const SkyBlue = Color::RGBf(0.529f, 0.808f, 0.922f);
        Color const SeaGreen = Color::RGBf(0.180f, 0.545f, 0.341f);
        Color const Gold = Color::RGBf(1.000f, 0.843f, 0.000f);
        Color const DarkOliveGreen = Color::RGBf(0.553f, 0.352f, 0.035f);
        Color const ForestGreen = Color::RGBf(0.133f, 0.545f, 0.133f);
        Color const OliveDrab = Color::RGBf(0.420f, 0.557f, 0.137f);
        Color const LightPink = Color::RGBf(1.000f, 0.713f, 0.757f);
        Color const AntiqueWhite = Color::RGBf(0.980f, 0.922f, 0.843f);
        Color const Snow = Color::RGBf(1.000f, 0.980f, 0.980f);
        Color const OldLace = Color::RGBf(0.992f, 0.961f, 0.902f);
        Color const LavenderBlush = Color::RGBf(1.000f, 0.941f, 0.960f);
        Color const PeachPuff = Color::RGBf(1.000f, 0.855f, 0.725f);
        Color const MistyRose = Color::RGBf(1.000f, 0.894f, 0.882f);
        Color const Moccasin = Color::RGBf(1.000f, 0.894f, 0.710f);
        Color const PapayaWhip = Color::RGBf(1.000f, 0.937f, 0.835f);
        Color const Seashell = Color::RGBf(1.000f, 0.961f, 0.933f);
        Color const Linen = Color::RGBf(0.980f, 0.941f, 0.902f);
        Color const LemonChiffon = Color::RGBf(1.000f, 0.980f, 0.804f);
        Color const Cornsilk = Color::RGBf(1.000f, 0.973f, 0.863f);
        Color const BlanchedAlmond = Color::RGBf(1.000f, 0.925f, 0.701f);
        Color const Wheat = Color::RGBf(0.961f, 0.871f, 0.702f);
        Color const BurlyWood = Color::RGBf(0.871f, 0.722f, 0.529f);
        Color const Tan = Color::RGBf(0.824f, 0.706f, 0.549f);
        Color const Peru = Color::RGBf(0.804f, 0.522f, 0.247f);
        Color const Chocolate = Color::RGBf(0.824f, 0.412f, 0.118f);
        Color const SaddleBrown = Color::RGBf(0.545f, 0.271f, 0.075f);
        Color const Sienna = Color::RGBf(0.627f, 0.322f, 0.176f);
        Color const RosyBrown = Color::RGBf(0.737f, 0.560f, 0.560f);
        Color const IndianRed = Color::RGBf(0.804f, 0.361f, 0.361f);
        Color const LightCoral = Color::RGBf(0.941f, 0.502f, 0.502f);
        Color const DarkRed = Color::RGBf(0.545f, 0.000f, 0.000f);
        Color const Firebrick = Color::RGBf(0.698f, 0.132f, 0.203f);
        Color const DarkMagenta = Color::RGBf(0.545f, 0.000f, 0.545f);
        Color const MediumPurple = Color::RGBf(0.576f, 0.439f, 0.859f);
        Color const PurpleHeart = Color::RGBf(0.444f, 0.259f, 0.597f);
        Color const Plum = Color::RGBf(0.867f, 0.627f, 0.867f);
        Color const Orchid = Color::RGBf(0.855f, 0.439f, 0.839f);
        Color const Thistle = Color::RGBf(0.847f, 0.749f, 0.847f);
        Color const PaleVioletRed = Color::RGBf(0.859f, 0.439f, 0.576f);
        Color const MediumOrchid = Color::RGBf(0.729f, 0.333f, 0.827f);
        Color const DarkSlateBlue = Color::RGBf(0.282f, 0.239f, 0.545f);
        Color const SlateBlue = Color::RGBf(0.416f, 0.353f, 0.804f);
        Color const MediumSlateBlue = Color::RGBf(0.482f, 0.408f, 0.933f);
        Color const CornflowerBlue = Color::RGBf(0.393f, 0.584f, 0.929f);
        Color const RoyalBlue = Color::RGBf(0.255f, 0.412f, 0.882f);
        Color const DodgerBlue = Color::RGBf(0.118f, 0.565f, 1.000f);
        Color const LightSkyBlue = Color::RGBf(0.529f, 0.808f, 0.980f);

        namespace Palette
        {

            template <std::size_t N>
            class Palette
            {
            public:
                explicit Palette(std::array<Color, N> const &colors) : d_colors(colors) {}

                // Sample color by index (wraps around)
                inline Color const &sample(std::size_t index) const
                {
                    return d_colors[index % N];
                }

                inline Color const &sample() const
                {
                    return d_colors[Nilib::RNG::rand() % N];
                }

                inline Color const &operator[](std::size_t index) const
                {
                    return sample(index);
                }

                inline size_t size() { return N; }

            protected:
                Palette() = default;
                std::array<Color, N> d_colors;
            };

            template <std::size_t N>
            struct Random : public Palette<N>
            {
                Random()
                {
                    for (size_t i = 0; i < N; i++)
                        Palette<N>::d_colors[i] = Nilib::Color::RandomHSL();
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