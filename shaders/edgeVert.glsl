#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in float inID;
layout (location = 1) in vec3 position;

out float pointSize;
flat out uint ID;//flat prevents interpolation, every fragment gets the same ID

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    ID = uint(inID);
}