#version 330 core
layout(location = 0) in vec2 aPosition;

layout(std140) uniform Camera {
    mat4 view;
    mat4 viewWithoutTranslation;
    mat4 projection;
    vec3 cameraPosition;
    float padding1;
};

out vec3 nearPoint;
out vec3 farPoint;

vec3 UnprojectPoint(float x, float y, float z) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    nearPoint = UnprojectPoint(aPosition.x, aPosition.y, 0.0);
    farPoint = UnprojectPoint(aPosition.x, aPosition.y, 1.0);
    gl_Position = vec4(aPosition, 0.0, 1.0); 
}