#version 330 core
out vec4 Fragment_Col;

in float outCol;

void main()
{
	Fragment_Col = vec4(outCol, outCol, outCol, 0.6);
}
