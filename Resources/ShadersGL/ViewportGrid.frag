#version 330 core
in vec3 nearPoint;
in vec3 farPoint;

layout(std140) uniform Camera {
    mat4 view;
    mat4 viewWithoutTranslation;
    mat4 projection;
    vec3 cameraPosition;
    float padding1;
};

out vec4 FragColor;

float near = 0.01;
float far = 10.0;


vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);

    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

    float line = min(grid.x, grid.y);

    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);

    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    if(fragPos3D.x > -2.0 * minimumx && 2.0 * fragPos3D.x < minimumx)
        color = vec4(0.0, 0.0, 1.0, 0.0);
    if(fragPos3D.z > -2.0 * minimumz && 2.0 * fragPos3D.z < minimumz)
        color = vec4(1.0, 0.0, 0.0, 1.0);
    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = projection * view * vec4(pos, 1.0);
    float ndcDepth = clip_space_pos.z / clip_space_pos.w;
    return ndcDepth * 0.5 + 0.5;
}

float computeLinearDepth(vec3 pos) {
  vec4 clip_space_pos = projection * view * vec4(pos, 1.0);
  float clip_space_depth = (clip_space_pos.z / clip_space_pos.w);
  float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); 
  return linearDepth / far; 
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    FragColor = (grid(fragPos3D, 1, true) + grid(fragPos3D, 0.1, true)) * float(t > 0);
    FragColor.a *= fading;
}
