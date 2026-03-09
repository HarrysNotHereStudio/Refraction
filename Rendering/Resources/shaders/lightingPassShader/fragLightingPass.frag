#version 450

layout(std140, binding = 0) uniform DefaultUniformBlock {
	mat4 viewMatrix;
	vec3 viewPos;
	mat4 projMatrix;
} ubo;

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuseSpecular;

struct Light {
	vec3 Position;
	vec3 Color;
};

const int MAXLIGHTS = 64;
uniform int LIGHTCOUNT;
uniform Light lights[MAXLIGHTS];

void main() {
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Diffuse = texture(gDiffuseSpecular, TexCoord).rgb;
    float Specular = texture(gDiffuseSpecular, TexCoord).a;

	vec3 lighting = Diffuse *.1; // Ambient
	vec3 viewDir = normalize(ubo.viewPos - FragPos);
	for (int i = 0; i < LIGHTCOUNT; i++) {
		vec3 lightDir = normalize(lights[i].Position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
		lighting += diffuse;
	};

	FragColor = vec4(lighting, 1.0);
}