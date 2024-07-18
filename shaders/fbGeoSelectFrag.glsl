#version 400 core

in vec2 texCoords;

uniform sampler2D screenTexture;

out vec4 outputColor;

void main()
{
    //gl_FragColor = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);
    outputColor = texture(screenTexture, texCoords);
}