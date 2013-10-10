#version 330 core

precision highp float;

in vec4 vcolor;
out vec4 fcolor;

void main()
{
	fcolor = vcolor;
}
