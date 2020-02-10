#version 330 core
out vec4 Fragment_Col;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in float outCol;
in vec3 Normal;
in vec3 FragPos;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = diffuse * objectColor;
	Fragment_Col = vec4(result, 1.0);
	// Fragment_Col = vec4(1.0, 1.0, 1.0, 1.0);
}
