#version 330 core

layout(location = 0) in vec3 position;  // Vertex position
layout(location = 1) in vec4 vertexColor;  // Vertex color (with alpha)

out vec4 fragColor;  // Output color to fragment shader

// uniform vec3 camera;

void main() {


    gl_Position = vec4(position, 1.0);


    fragColor = vertexColor;  // Pass the color with alpha to fragment shader
}
