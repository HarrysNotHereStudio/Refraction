#version 450

layout(std140, binding = 0) uniform DefaultUniformBlock {
	mat4 viewMat;
	vec3 viewPos;
	mat4 projMat;
} ubo;

layout (location = 0) in vec3 aPos;

uniform mat4 modelTransform;

void main() {
	gl_Position = ubo.projMat * ubo.viewMat * modelTransform * vec4(aPos, 1);
}