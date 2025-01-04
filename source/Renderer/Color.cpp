#include "Nilib/Renderer/Color.h"
#include "Nilib/Math/RNG.hpp"

using namespace Nilib;

Color::Color()
: store{0xe5,0xe5,0xe5,0xff} {}

void Color::RGBAf(float *R, float *G, float *B, float *A) const
{
    RGBf(R, G, B);
    *A = static_cast<float>(store.rgba[3] / 255.0f);
}
void Color::RGBf(float *R, float *G, float *B) const
{
    *R = static_cast<float>(store.rgba[0] / 255.0f);
    *G = static_cast<float>(store.rgba[1] / 255.0f);
    *B = static_cast<float>(store.rgba[2] / 255.0f);
}

Color Color::RGBf(float R, float G, float B)
{
    return Color::RGBAf(R, G, B, 0.9f);
}

Color Color::RGBAf(float R, float G, float B, float A)
{
    return Color(R * 255, G * 255, B * 255, A * 255);
}

Color::Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
: store{R, G, B, A} {}


Color Color::RGBA(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
    return Color(R, G, B, A);
}

#undef RGB
Color Color::RGB(uint8_t R, uint8_t G, uint8_t B)
{
    return Color(R, G, B, 255);
}

Color Color::RandomRGB()
{
    uint32_t val = RNG::rand();
    uint8_t r = (val & 0x000000ff);
    uint8_t g = (val & 0x0000ff00) >> 8;
    uint8_t b = (val & 0x00ff0000) >> 16;
    uint8_t a = (val & 0xff000000) >> 24;
    return Color(r, g, b, a);
}

Color Color::RandomHSL()
{
    return Color::HSL(RNG::prob() * 360.0f, RNG::uniform(0.5, 0.8), RNG::uniform(0.5, 0.8), 200U);    
}
    
// Randomize hue, saturation, and lightness
Color Color::HSL(float h, float s, float l, uint8_t alpha)
{
    // Normalize hue to be between 0 and 360
    h = std::fmod(h, 360.0f);
    if (h < 0) h += 360.0f;

    // Chroma
    float c = (1 - std::fabs(2 * l - 1)) * s;
    
    // X value
    float x = c * (1 - std::fabs(std::fmod(h / 60.0f, 2) - 1));

    // m value
    float m = l - c / 2;

    // RGB intermediate values
    float r_prime, g_prime, b_prime;
    if (h >= 0 && h < 60) {
        r_prime = c;
        g_prime = x;
        b_prime = 0;
    } else if (h >= 60 && h < 120) {
        r_prime = x;
        g_prime = c;
        b_prime = 0;
    } else if (h >= 120 && h < 180) {
        r_prime = 0;
        g_prime = c;
        b_prime = x;
    } else if (h >= 180 && h < 240) {
        r_prime = 0;
        g_prime = x;
        b_prime = c;
    } else if (h >= 240 && h < 300) {
        r_prime = x;
        g_prime = 0;
        b_prime = c;
    } else {
        r_prime = c;
        g_prime = 0;
        b_prime = x;
    }

    // Final RGB values
    uint8_t r = static_cast<uint8_t>((r_prime + m) * 255);
    uint8_t g = static_cast<uint8_t>((g_prime + m) * 255);
    uint8_t b = static_cast<uint8_t>((b_prime + m) * 255);

    // Return the RGBA result
    return Color(r, g, b, alpha);
}
