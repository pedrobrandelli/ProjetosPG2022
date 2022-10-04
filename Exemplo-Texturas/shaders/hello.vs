#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coord;

out vec4 vertexColor;
out vec2 texCoord;

uniform mat4 projection; //matriz de projeção
uniform mat4 model; //matriz de modelo

void main()
{
	//...pode ter mais linhas de código aqui!
	gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);
	texCoord = vec2(tex_coord.x, 1-tex_coord.y);	
	vertexColor = vec4(color,1.0);
}