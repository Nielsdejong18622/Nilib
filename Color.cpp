#include "Color.h"
#include "RNG.hpp"

#include <math.h>

/*Color::Color(unsigned char R, unsigned char G, unsigned char B) : 
    R(static_cast<unsigned char>(R / 255)),
    G(static_cast<unsigned char>(G / 255)), 
    B(static_cast<unsigned char>(B / 255)) { }
*/

Color::Color(float R, float G, float B) : 
    R(R),
    G(G), 
    B(B) {}

Color Color::random()
{ 
    return Color(RNG::prob(), RNG::prob(), RNG::prob());
}

// Static constructors.
Color Color::GreyTone(float size_t) {
    return Color(1 / size_t, 1 / size_t, 1 / size_t);
}

Color Color::HSL(float const hue, float const saturation, float const lightness) {
    float C = lightness * saturation;
    float X = C * (1 - std::abs(std::fmod(hue / 60.0, 2) - 1));
    float m = lightness - C;
    float r, g, b;
    if (hue < 60) {
        r = C;
        g = X;
        b = 0;
    } else if (hue < 120) {
        r = X;
        g = C;
        b = 0;
    } else if (hue < 180) {
        r = 0;
        g = C;
        b = X;
    } else if (hue < 240) {
        r = 0;
        g = X;
        b = C;
    } else if (hue < 300) {
        r = X;
        g = 0;
        b = C;
    } else {
        r = C;
        g = 0;
        b = X;
    }
    return Color(r + m, g + m, b + m);
}

// Static pallette creator.
std::vector<Color> Color::Greytones(size_t const numtones) {
    std::vector<Color> colors(numtones);
    for (size_t i = 0; i < numtones; ++i) {
        colors[i] =GreyTone(i / numtones);
    }
    return colors;
}


std::vector<Color> Color::HSLcolors(size_t const numColors) {
    std::vector<Color> colors(numColors);
    for (size_t i = 0; i < numColors; ++i) {
        colors[i] = HSL((60 + i * 360) / numColors, 0.6, 1.0);
    }
    return colors;
}
