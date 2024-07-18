#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in float ID;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 inNormal;

out vec3 normal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    //This is to correct for non-uniform scaling of the object,
    //as that would make the normal vector no longer perpendicular to the surface
    //normal = mat3(transpose(inverse(model))) * inNormal;  
    normal = inverse(transpose(mat3(model))) * normalize(inNormal);
}