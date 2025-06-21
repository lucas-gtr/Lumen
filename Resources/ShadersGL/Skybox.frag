#version 330 core
in vec2 fragCoord;
in mat4 invViewWithoutTranslation;
in mat4 invProjection;

out vec4 fragColor;

uniform sampler2D skybox;

#define PI 3.14159265358979323846

vec3 getViewRay(vec2 ndc)
{
    vec4 clip = vec4(ndc, -1.0, 1.0);
    
    vec4 view = invProjection * clip;
    view.z = -1.0;
    view.w = 0.0;

    vec3 worldDir = normalize((invViewWithoutTranslation * view).xyz);
    return worldDir;
}

void main()
{
    vec2 ndc = fragCoord;
    vec3 viewDir = getViewRay(ndc);

    vec2 uv = vec2(atan(viewDir.z, viewDir.x) / (2.0 * PI) + 0.5, 
                   0.5 - asin(viewDir.y) / PI);
    fragColor = texture(skybox, uv);
}
