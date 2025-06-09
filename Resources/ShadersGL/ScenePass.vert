#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

uniform mat4 model;
uniform mat3 normalMatrix;

layout(std140) uniform Camera
{
    mat4 view;
    mat4 viewWithoutTranslation;
    mat4 projection;
    vec3 cameraPosition;
    float padding1;
};

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragTangent;
out vec3 fragBitangent;

void main() {
    vec4 worldPosition = model * vec4(inPosition, 1.0);

    fragPosition = worldPosition.xyz;

    fragNormal = normalize(normalMatrix * inNormal);
    fragTangent = normalize(normalMatrix * inTangent);
    fragBitangent = normalize(normalMatrix * inBitangent);
    
    fragTexCoord = inTexCoord;
    
    gl_Position = projection * view * worldPosition;
}