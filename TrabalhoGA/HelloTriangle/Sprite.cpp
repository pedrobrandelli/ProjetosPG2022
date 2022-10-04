#include "Sprite.h"

Sprite::Sprite()
{
	idAnim = 0;
	idFrame = 0;
	escala = glm::vec3(1.0, 1.0, 1.0);
	pos = glm::vec3(0.0, 0.0, 0.0);
	nAnims = 0;
	nFrames = 0;
}

void Sprite::inicializar(int textura, glm::vec3 escala, int nAnims, int nFrames)
{
	this->textura = textura;

	//AQUI criamos a geometria do Sprite (um quadrado de lado 1, centrado na origem)
	this->nAnims = nAnims;
	this->nFrames = nFrames;

	offsetUV.x = 1.0 / nFrames; //fatia para somar na coordenada de textura 
	offsetUV.y = 1.0 / nAnims;

	float vertices[] = {
		// posicoes          // cores           // coordenadas de textura
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   offsetUV.x, offsetUV.y, // topo da direita
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   offsetUV.x, 0.0, // base da direita
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0, // topo da esquerda
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, offsetUV.y  // base da esquerda 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	this->escala.x = escala.x /nFrames;
	this->escala.y = escala.y / nAnims;

}

void Sprite::setShader(Shader* shader)
{
	this->shader = shader;
	shader->Use();
}

void Sprite::setTransform()
{
	//shader->Use();

	setTranslacao(pos);
	setEscala(escala,false);
	// Get their uniform location
	//GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	// Pass them to the shaders
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Sprite::setRotacao(float angle, glm::vec3 axis, bool reset)
{
	if (reset) model = glm::mat4();
	model = glm::rotate(model, angle, axis);
}

void Sprite::setTranslacao(glm::vec3 displacements, bool reset)
{
	if (reset) model = glm::mat4();
	model = glm::translate(model, displacements);
	pos = displacements;
}

void Sprite::setEscala(glm::vec3 scaleFactors, bool reset)
{
	if (reset) model = glm::mat4();
	model = glm::scale(model, scaleFactors);
	escala = scaleFactors;
}

void Sprite::desenhar()
{
	//shader->Use();

	//atualizarAnimacao(); 
	
	//seta a textura
	glBindTexture(GL_TEXTURE_2D, textura);
	//glUniform1i(glGetUniformLocation(shader->ID, "tex_buffer"), 0);

	//considerando que possui EBO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sprite::atualizar()
{
	//shader->Use();
	setTransform();
}

void Sprite::atualizarAnimacao()
{
	// Recupera a localização do uniform
	GLint offsetLoc = glGetUniformLocation(shader->ID, "offsetUV");

	idFrame = (idFrame + 1) % nFrames;

	// Passa o valor para o shader, atualizado
	glm::vec2 _offsetUV = glm::vec2(offsetUV.x * idFrame, offsetUV.y * idAnim);
	glUniform2f(offsetLoc, _offsetUV.x, _offsetUV.y);
}
