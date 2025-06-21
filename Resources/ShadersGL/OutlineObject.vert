#version 330 core
layout(location = 0) in vec3 inPosition;

uniform mat4 model;
uniform float outline_scale;

layout(std140) uniform Camera
{
    mat4 view;
    mat4 viewWithoutTranslation;
    mat4 projection;
    vec3 cameraPosition;
    float padding1;
};

void main()
{
    vec3 scaledPos = inPosition * outline_scale;
    gl_Position = projection * view * model * vec4(scaledPos, 1.0);
}
