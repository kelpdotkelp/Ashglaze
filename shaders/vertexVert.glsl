#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//layout (location = 2) in float inID;
layout (location = 0) in float inID;
layout (location = 1) in vec3 position;

out float pointSize;
flat out uint ID;//flat prevents interpolation, every fragment gets the same ID

void main()
{
    pointSize = 25;

    gl_Position = projection * view * model * vec4(position, 1.0);
    gl_PointSize = pointSize;

    ID = uint(inID);
}