#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 inTextureCoordinate;
layout(location = 2) in vec3 normal;

uniform mat4 model, view, projection;

out vec2 outTextureCoordinate;
out vec3 positionEye, normalEye;

void main() {
	outTextureCoordinate = inTextureCoordinate;
	positionEye = vec3(view * model * vec4(position, 1.0));
	normalEye = vec3(view * model * vec4(normal, 0.0));
	gl_Position = projection * positionEye;
}