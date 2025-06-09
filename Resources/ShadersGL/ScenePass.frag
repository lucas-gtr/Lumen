#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragTangent; 
in vec3 fragBitangent;

out vec4 fragColor;

const int numSampleOffsets = 20;
const vec3 sampleOffsetDirections[numSampleOffsets] = vec3[]
(
	vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
	vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

struct DirectionalLight {
  vec3 direction;
  vec3 color;
  mat4 lightSpaceMatrix;
};

struct PointLight {
  vec3 position;
  vec3 color;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float innerCutOff;
  float outerCutOff;
  vec3 color;
  mat4 lightSpaceMatrix;
};

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 2

layout(std140) uniform Camera
{
    mat4 view;
    mat4 viewWithoutTranslation;
    mat4 projection;
    vec3 cameraPosition;
    float padding1;
};

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D shadowMap2D;
uniform samplerCube shadowMapCube;
uniform float shadowMapCubeFarPlane;

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform DirectionalLight dirLights[MAX_DIRECTIONAL_LIGHTS];
uniform sampler2D dirShadowMaps[MAX_DIRECTIONAL_LIGHTS];

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

const float ambientStrengthDirectional = 0.05f;
const float ambientStrengthPoint = 0.025f;
const float ambientStrengthSpot = 0.025f;

const float specularShininess = 128.0f;

float biasMinValue = 0.001;
float biasMaxValue = 0.01;

float getShadowFactor2D(vec3 normal, vec3 lightDir, vec4 fragLightSpacePosition, vec4 shadowMapRegion) {
	vec3 projCoords = fragLightSpacePosition.xyz / fragLightSpacePosition.w;
	projCoords = projCoords * 0.5 + 0.5;
	vec2 uv = mix(shadowMapRegion.xy, shadowMapRegion.zw, projCoords.xy);
	float currentDepth = projCoords.z;
	float bias = max(biasMaxValue * (1.0 - dot(normal, lightDir)), biasMinValue);

	float shadow = 0.0;
	float radius = 0.001;
	for (int i = 0; i < numSampleOffsets; ++i) {
		vec2 offset = sampleOffsetDirections[i].xy * radius;
		vec2 sampleUV = uv + offset;
		float closestDepth = texture(shadowMap2D, sampleUV).r;
		if (currentDepth - bias <= closestDepth)
			shadow += 1.0;
	}
  shadow /= float(numSampleOffsets);
	return shadow;
}

float getShadowFactorCube(vec3 normal, vec3 lightDir, vec3 fragPos, vec3 lightPos) {  
  vec3 fragToLight = fragPosition - lightPos;

	float currentDepth = length(fragToLight) / shadowMapCubeFarPlane;

	float bias = max(biasMaxValue * (1.0 - dot(normal, lightDir)), biasMinValue);

  float shadow = 0.0;
	float radius = 0.01;

	for (int i = 0; i < numSampleOffsets; ++i) {
    vec3 offsetDir = fragToLight + sampleOffsetDirections[i] * radius;
    float closestDepth = texture(shadowMapCube, offsetDir).r;
	  if (currentDepth - bias <= closestDepth)
			shadow += 1.0;
  }

  shadow /= float(numSampleOffsets);
	return shadow;
}

vec3 applyDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, int index) {
  vec3 lightDir = -light.direction;
  vec3 halfwayDir = normalize(lightDir + viewDir);

  float diffuse = max(dot(normal, lightDir), 0.0);
  float specular = pow(max(dot(normal, halfwayDir), 0.0), specularShininess);

  vec4 fragLightSpacePosition = light.lightSpaceMatrix * vec4(fragPosition, 1.0);
  vec4 region = vec4(0.5 * index, 0.0, 0.5 + 0.5 * index, 0.5);
  float shadowFactor = getShadowFactor2D(normal, lightDir, fragLightSpacePosition, region);

  return (ambientStrengthDirectional + shadowFactor * (diffuse + specular)) * light.color;
}

vec3 applyPointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);

    float diffuse = max(dot(normal, lightDir), 0.0);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), specularShininess);

    float shadowFactor = getShadowFactorCube(normal, lightDir, fragPos, light.position);

    return (ambientStrengthPoint + shadowFactor * (diffuse + specular)) * attenuation * light.color;
}

vec3 applySpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir, int index)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);

    float theta = dot(lightDir, -light.direction);
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diffuse = max(dot(normal, lightDir), 0.0);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), specularShininess);

    vec4 fragLightSpacePosition = light.lightSpaceMatrix * vec4(fragPosition, 1.0);
    vec4 region = vec4(0.5 * index, 0.5, 0.5 + 0.5 * index, 1.0);

    float shadowFactor = getShadowFactor2D(normal, lightDir, fragLightSpacePosition, region);

    return intensity * (ambientStrengthSpot + shadowFactor * (diffuse + specular)) * attenuation * light.color;
}

void main() {
  vec3 T = normalize(fragTangent);
  vec3 B = normalize(fragBitangent);
  vec3 N = normalize(fragNormal);
  mat3 TBN = mat3(T, B, N);

  vec3 texNormal = texture(normalTexture, fragTexCoord).rgb;
  texNormal = normalize(texNormal * 2.0 - 1.0); // from [0,1] to [-1,1]
  vec3 normal = normalize(TBN * texNormal);

  vec3 viewDir = normalize(cameraPosition - fragPosition);

  vec3 lighting = vec3(0.0);

  for (int i = 0; i < numDirLights; ++i)
    lighting += applyDirectionalLight(dirLights[i], normal, viewDir, i);

  for (int i = 0; i < numPointLights; ++i)
    lighting += applyPointLight(pointLights[i], fragPosition, normal, viewDir);

  for (int i = 0; i < numSpotLights; ++i)
    lighting += applySpotLight(spotLights[i], fragPosition, normal, viewDir, i);

  fragColor = vec4(lighting, 1.0) * texture(diffuseTexture, fragTexCoord);
}