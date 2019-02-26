#version 330 core
layout (location = 0) in vec3 lineVertex;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform vec3 color;
out vec3 fColor;

void main()
{
    gl_Position =  projection * view *  vec4(lineVertex, 1.0);
	fColor = color;
}