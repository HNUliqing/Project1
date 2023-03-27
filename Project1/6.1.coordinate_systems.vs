#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	if(aPos.z!=0)
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
	else 
	{
		gl_Position=vec4(aPos, 1.0);
	}
}