#version 430 core
out vec4 FragColor;

in vec4 TexCoord;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	FragColor=TexCoord;
}