#version 330 core
layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float outCol;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * model * vec4(posAttr, 1.0);
	outCol = 1.0 - abs(posAttr.z / 100.0f);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(posAttr, 1.0));
}
