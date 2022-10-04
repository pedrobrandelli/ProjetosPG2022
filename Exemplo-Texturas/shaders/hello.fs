#version 450

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D tex_buffer;

//Texture coords offsets for animation
uniform vec2 offset;


out vec4 color;

void main()
{
	color = texture(tex_buffer,texCoord+offset);
}