#version 400 core

uniform bool renderIDMode = false;
uniform vec3 color;

uniform int selectedID = -1;//-1 maps to no object selected.
uniform vec3 selectedColor;

in float pointSize;
flat in uint ID;//flat prevents interpolation, every fragment gets the same ID

out vec4 outputColor;

void main()
{
    vec2 pos = vec2(gl_PointCoord.x - 0.5, gl_PointCoord.y - 0.5);

    if (sqrt(dot(pos, pos)) > 0.5)
    {
        discard;
    }
    else
    {
        if (!renderIDMode)
        {
            if (selectedID != ID)
                outputColor = vec4(color, 1.0);
            else
                outputColor = vec4(selectedColor, 1.0);
        }
        else
        {
            float r = ((ID & 0x000000FF) >> 0)/255.0;
            float g = ((ID & 0x0000FF00) >>  8)/255.0;
            float b = ((ID & 0x00FF0000) >> 16)/255.0;
            outputColor = vec4(r, g, b, 1.0);
        }
    }
}