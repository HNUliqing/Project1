#version 430	core

//layout(location = 0) in vec3 position;
//
//out vec4 vertexColor;
//
//void main() {
//    gl_Position = vec4(position, 1.0);
//
//    vertexColor = vec4(position * 0.5 + 0.5, 1.0);
//}
layout(location = 0) in vec3 position;


out vec4 fColor;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//void main() {
//    gl_Position = projection * view * model * vec4(position, 1.0);
//    fColor = vec4(position * 0.5 + 0.5, 1.0);
//}

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position, 1.0);
	fColor = vec4(position * 0.5 + 0.5, 1.0);
}