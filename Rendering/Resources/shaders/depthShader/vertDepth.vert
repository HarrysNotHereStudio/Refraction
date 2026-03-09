#version 450

layout(std140, binding = 0) uniform DefaultUniformBlock {
	mat4 view;
	mat4 proj;
} ubo;

uniform mat4 modelTransform;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
	gl_Position = ubo.proj * ubo.view * modelTransform * vec4(aPos, 1);
	TexCoord = aTexCoord;
}