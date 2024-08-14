#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float pointSize = 25;
uniform float poitnSizeMin = 5;

uniform vec3 cameraPosition;

//layout (location = 2) in float inID;
layout (location = 0) in float inID;
layout (location = 1) in vec3 position;

flat out uint ID;//flat prevents interpolation, every fragment gets the same ID

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vec3 transformedPos = (model * vec4(position, 1.0)).xyz;
    gl_PointSize = max(pointSize - pow(distance(cameraPosition, transformedPos), 2), poitnSizeMin);

    ID = uint(inID);
}