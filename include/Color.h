#ifndef _COLOR_H
#define _COLOR_H

#include <vector>

struct Color {

public:
    float R = 0.2;
    float G = 0.2;
    float B = 0.2;

    

    Color(float R = 0, float G = 0, float B = 0);
    //Color(unsigned char R, unsigned char G, unsigned char B);

    // Generate a deterministic array of HSL colors. 
    static std::vector<Color> HSLcolors(size_t num);
    static std::vector<Color> Greytones(size_t num);

    // Create a HSL color.
    static Color HSL(float const hue, float const saturation, float const lightness);

    // Create a grey tone.
    static Color GreyTone(float size_t);

    // Create a random color.
    static Color random();

};

namespace Colors{
    

Color const Gray = Color(0.411, 0.411, 0.411);
Color const Grey = Color(0.411, 0.411, 0.411);
Color const Red = Color(1.000, 0.000, 0.000);
Color const Blue = Color(0.000, 0.000, 1.000);
Color const Green = Color(0.000, 1.000, 0.000);
Color const Yellow = Color(1.000, 1.000, 0.000);
Color const Orange = Color(1.000, 0.647, 0.000);
Color const Purple = Color(0.502, 0.000, 0.502);
Color const Pink = Color(1.000, 0.753, 0.796);
Color const Black = Color(0.000, 0.000, 0.000);
Color const White = Color(1.000, 1.000, 1.000);
Color const Brown = Color(0.647, 0.165, 0.165);
Color const Cyan = Color(0.000, 1.000, 1.000);
Color const Magenta = Color(1.000, 0.000, 1.000);
Color const Turquoise = Color(0.251, 0.878, 0.816);
Color const Indigo = Color(0.294, 0.000, 0.510);
Color const Maroon = Color(0.502, 0.000, 0.000);
Color const Olive = Color(0.502, 0.502, 0.000);
Color const Teal = Color(0.000, 0.502, 0.502);
Color const Lavender = Color(0.902, 0.902, 0.980);
Color const Coral = Color(1.000, 0.498, 0.314);






};
#endif