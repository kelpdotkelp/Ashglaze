#version 400 core

layout (location = 0) in float inID;
layout (location = 1) in vec3 position;

flat out uint ID;//flat prevents interpolation, every fragment gets the same ID

void main()
{
    gl_Position = vec4(position, 1.0);

    ID = uint(inID);
}