#version 330 core

in vec4 fragColor;  // Input color from vertex shader (including alpha)
out vec4 finalColor;  // Final color of the fragment (pixel)

void main() {
    finalColor = fragColor;  // Use the input color directly (including alpha)
}
