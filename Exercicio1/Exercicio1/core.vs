#version 450

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 colors;
out vec3 color;

void main () {
	color = colors;
	gl_Position = vec4 (positions, 1.0);
}