#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 positionEye, normalEye;

uniform sampler2D texture;
uniform float colorPercentage, ambientIntensity, lightIntensity, shininess;
uniform vec3 camera, objectReflection, surfaceReflection, specularIntensity;

void main() {
	vec3 ambientLight = ambientIntensity * objectReflection;

	float distanceToLight = length(camera - positionEye);
	vec3 lightDirection = normalize(camera - positionEye);
	float diffuseIntensity = max(dot(lightDirection, normalEye), 0.0f);
	vec3 diffuseLight = lightIntensity * surfaceReflection * diffuseIntensity;

	float attenuation = 1 / pow(distanceToLight, 2);

	vec3 viewNormalized = normalize(-positionEye);
	vec3 reflectionEye = reflect(-lightDirection, normalize(normalEye));
	float dotProductSpecular = max(dot(reflectionEye, viewNormalized), 0.0f);
	float specularFactor = pow(dotProductSpecular, shininess);

	float specularLight = specularIntensity * specularFactor;

	float light = ambientLight + attenuation * (diffuseLight + specularLight);

    FragColor = light * mix(texture(texture, TexCoords), vec4(0.0f, 0.0f, 1.0f, 0.0f), colorPercentage);
}