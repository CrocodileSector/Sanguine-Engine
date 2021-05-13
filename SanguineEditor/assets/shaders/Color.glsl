#type vertex
#version 330 core

layout(location = 1) in vec3 a_Color;
out vec3 v_Color;

void main()
{
	v_Color = a_Color;
}

#type fragment
#version 330 core

layout(location = 0) out vec3 color;

in vec3 v_Color;

void main()
{
	color = v_Color;
}