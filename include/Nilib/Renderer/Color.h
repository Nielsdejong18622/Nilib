#ifndef _COLOR_H
#define _COLOR_H

#include <vector>
#include <array>
#include <math.h>
#include <stdint.h>
#include <ostream>

namespace Nilib
{
    
    // A humanly discernable Color.
    // Supports RGB(a), HSL, HSV, 
    // AnsiConsole (Windows and Linux), TrueColor,
    // Int
    class Color {

        union Storage {
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

        friend std::ostream& operator<<(std::ostream& os, const Color& color) {
            float R, G, B, A;
            color.RGBAf(&R, &G, &B, &A);
            return os << "<Color " << R << ' ' << G << ' ' << B << ' '<< A << '>';
        }

    };

    namespace Colors {
        Color const Grey = Color::HSL(0.411, 0.411, 0.411);
        Color const Gray = Color::HSL(0.411, 0.411, 0.411);
        Color const Red = Color::HSL(1.000, 0.000, 0.000);
        Color const Blue = Color::HSL(0.000, 0.000, 1.000);
        Color const Green = Color::HSL(0.000, 1.000, 0.000);
        Color const Yellow = Color::HSL(1.000, 1.000, 0.000);
        Color const Orange = Color::HSL(1.000, 0.647, 0.000);
        Color const Purple = Color::HSL(0.502, 0.000, 0.502);
        Color const Pink = Color::HSL(1.000, 0.753, 0.796);
        Color const Black = Color::HSL(0.000, 0.000, 0.000);
        Color const White = Color::HSL(1.000, 1.000, 1.000);
        Color const Brown = Color::HSL(0.647, 0.165, 0.165);
        Color const Cyan = Color::HSL(0.000, 1.000, 1.000);
        Color const Magenta = Color::HSL(1.000, 0.000, 1.000);
        Color const Turquoise = Color::HSL(0.251, 0.878, 0.816);
        Color const Indigo = Color::HSL(0.294, 0.000, 0.510);
        Color const Maroon = Color::HSL(0.502, 0.000, 0.000);
        Color const Olive = Color::HSL(0.502, 0.502, 0.000);
        Color const Teal = Color::HSL(0.000, 0.502, 0.502);
        Color const Lavender = Color::HSL(0.902, 0.902, 0.980);
        Color const Coral = Color::HSL(1.000, 0.498, 0.314);
    };
} // namespace Nilib

#endif