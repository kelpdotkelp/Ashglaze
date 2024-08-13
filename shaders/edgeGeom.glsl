#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float lineWidth = 0.01;
uniform vec3 cameraDirection;

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

flat in uint ID[];

flat out uint outID;

void main()
{
    //Vector orthogonal both line and camera direction
    vec3 orthoDir = cross(cameraDirection, (gl_in[0].gl_Position - gl_in[1].gl_Position).xyz);
    orthoDir = orthoDir/length(orthoDir);

    gl_Position =  projection * view * model * vec4(gl_in[0].gl_Position.xyz + lineWidth*orthoDir, gl_in[0].gl_Position.w);
    outID = ID[0];
    EmitVertex();

    gl_Position =  projection * view * model * vec4(gl_in[1].gl_Position.xyz + lineWidth*orthoDir, gl_in[1].gl_Position.w);
    outID = ID[0];
    EmitVertex();

    gl_Position =  projection * view * model * vec4(gl_in[0].gl_Position.xyz - lineWidth*orthoDir, gl_in[0].gl_Position.w);
    outID = ID[0];
    EmitVertex();

    gl_Position =  projection * view * model * vec4(gl_in[1].gl_Position.xyz - lineWidth*orthoDir, gl_in[1].gl_Position.w);
    outID = ID[0];
    EmitVertex();

    EndPrimitive();
}