#version 330 core
in vec2 fragCoord;
out vec4 fragColor;

layout(std140) uniform Camera
{
    mat4 view;
    mat4 viewWithoutTranslation;
    mat4 projection;
    vec3 cameraPosition;
    float padding1;
};

uniform sampler2D skybox;

vec3 getViewRay(vec2 ndc, mat4 proj, mat4 viewRotOnly)
{
    vec4 clip = vec4(ndc, -1.0, 1.0);
    
    vec4 view = inverse(proj) * clip;
    view.z = -1.0; // for direction, not position
    view.w = 0.0;

    vec3 worldDir = normalize((inverse(viewRotOnly) * view).xyz);
    return worldDir;
}

void main()
{
    vec2 ndc = fragCoord;
    vec3 viewDir = getViewRay(ndc, projection, viewWithoutTranslation);

    vec2 uv = vec2(atan(viewDir.z, viewDir.x) / (2.0 * 3.14159265358979323846) + 0.5, 
                   0.5 - asin(viewDir.y) / 3.14159265358979323846);
    fragColor = texture(skybox, uv);
}
