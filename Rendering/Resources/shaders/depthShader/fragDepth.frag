#version 450

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texDiffuse1;

void main() {
	FragColor = vec4(gl_FragCoord.z / 10 * vec3(1.0), 1.0);
}