#version 400 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 inTexCoords;

uniform mat4 model;

out vec2 texCoords;

void main()
{
    gl_Position = model * vec4(position.x, position.y, 0.0, 1.0);
    texCoords = inTexCoords;
}