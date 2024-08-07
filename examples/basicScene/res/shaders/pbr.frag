#version 460

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec4 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragLightPos;
layout(location = 4) in vec3 fragViewPos;


layout(binding = 1) uniform ObjectBuffer
{
	vec3 camPos;
	vec3 lightPos;
	float metalic;
	float roughness;
	float ambientOcclusion;
} ubo;

layout(binding = 2) uniform sampler2D albedoTex;
layout(binding = 3) uniform sampler2D metalicRoughnessOcclusionTex;
layout(binding = 4) uniform sampler2D normalTex;
// layout(binding = 5) uniform sampler2D emissiveTex;

const float PI = 3.14159265;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	denom = max(denom, 0.000001);

	return num / denom;
}

float GeometrySchlickGGX(float NdotX, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotX;
	float denom = NdotX * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2  = GeometrySchlickGGX(NdotV, roughness);
	float ggx1  = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
	vec3 albedo = vec3(texture(albedoTex, fragTexCoord)) * vec3(fragColor);
	// albedo = pow(albedo, vec3(2.2));
	float metalic = texture(metalicRoughnessOcclusionTex, fragTexCoord).b;
	float roughness = texture(metalicRoughnessOcclusionTex, fragTexCoord).g;
	float ambientOcclusion = ubo.ambientOcclusion;

	vec3 normal = texture(normalTex, fragTexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 camPos = fragViewPos;
	vec3 lightPos = fragLightPos;
	vec3 lightColors = vec3(1.0);
	float lightStrength = 10.0;

	vec3 N = normalize(normal);
	vec3 V = normalize(camPos - fragPos);

	vec3 L = normalize(lightPos - fragPos);
    vec3 H = normalize(V + L);

	float distance = length(lightPos - fragPos);
	float attenuation = lightStrength / (distance * distance);
	vec3 radiance = lightColors * attenuation;

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalic);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metalic;

	vec3 numerator = NDF * G * F;
	float denominator = max(4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0), 0.0001);
	vec3 specular = numerator / denominator;

	float NdotL = max(dot(N, L), 0.0);
	vec3 BRDF = (kD * albedo / PI + specular) * radiance * NdotL;

	vec3 ambient = vec3(0.1) * albedo;
	vec3 color = BRDF + ambient;

	// color = color / (color + vec3(1.0));
	// color = pow(color, vec3(1.0/2.2));

	outColor = vec4(color, 1.0f);
}
