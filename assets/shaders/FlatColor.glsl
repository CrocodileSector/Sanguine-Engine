#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_Position;
out vec4 v_Color;

void main()
{
	v_Position = vec4(a_Position, 1.0);
	v_Color = a_Color;

	gl_Position = u_Projection * u_View * u_Model * v_Position;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Position;
in vec4 v_Color;

void main()
{
	color = v_Color;
}