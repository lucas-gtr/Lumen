#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int toneMapping;
uniform sampler2D screenTexture;

uniform float exposure; // For exposure-based tone mapping

void main()
{
  vec3 color = texture(screenTexture, TexCoords).rgb;

  switch (toneMapping) {
    case 0: // No tone mapping
      color = clamp(color, 0.0, 1.0);
      break;
    case 1: // Reinhard
      color = color / (color + vec3(1.0));
      break;
    case 2: // Exposure
      color = 1.0 - exp(-color * exposure);
      break;
    default:
      break;
  }

  // Gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  FragColor = vec4(color, 1.0);
}