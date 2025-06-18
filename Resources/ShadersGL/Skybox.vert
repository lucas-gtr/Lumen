
#version 330 core
layout(location = 0) in vec2 aPosition;

out vec2 fragCoord;
out mat4 invViewWithoutTranslation;
out mat4 invProjection;

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
    invViewWithoutTranslation = inverse(viewWithoutTranslation);
    invProjection = inverse(projection);
    
    fragCoord = aPosition;
    gl_Position = vec4(aPosition, 1.0, 1.0);
}