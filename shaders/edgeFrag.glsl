#version 400 core

uniform bool renderIDMode = false;
uniform vec3 color;

uniform int selectedID = -1;//-1 maps to no object selected.
uniform vec3 selectedColor;
uniform vec3 IDToRender = vec3(-1, -1, -1);

flat in uint ID;

out vec4 outputColor;

void main()
{
    if (uint(IDToRender.x) == ID ||
        uint(IDToRender.y) == ID ||
        uint(IDToRender.z) == ID)
    {
        if (!renderIDMode)//Visual rendering
        {
            if (selectedID != ID)
                outputColor = vec4(color, 1.0);
            else
                outputColor = vec4(0.0, 1.0, 0.0, 1.0);
                //outputColor = vec4(selectedColor, 1.0);
        }
        else//ID rendering
        {
            float r = ((ID & 0x000000FF) >> 0)/255.0;
            float g = ((ID & 0x0000FF00) >>  8)/255.0;
            float b = ((ID & 0x00FF0000) >> 16)/255.0;
            outputColor = vec4(r, g, b, 1.0);
        }
    }
    else
        discard;
}