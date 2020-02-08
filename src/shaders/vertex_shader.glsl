#version 330 core
layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 colorAttr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float outCol;

void main()
{
	gl_Position = projection * view * model * vec4(posAttr, 1.0);
	// gl_Position = vec4(colorAttr, 1.0);
	outCol = 1.0 - abs(posAttr.z / 100.0f);
}
