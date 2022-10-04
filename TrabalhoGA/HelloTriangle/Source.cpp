/* Trabalho do Grau A - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Criado por Rossana Baptista Queiroz
 * Adaptado por Gabriela Mezetti e Pedro Brandelli
 * para a disciplina de Processamento Gráfico - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 29/09/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB_IMAGE
#include "stb_image.h"


#include<string>

#include "Shader.h"

using namespace std;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

GLuint generateTexture(string filePath);

void drawObject(glm::mat4 matriz_modelo, glm::vec3 translacao, glm::vec3 escala, GLuint idTextura, GLuint VAO, Shader shader);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 675, HEIGHT = 1000;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	srand(time(NULL));

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho GA - Gabriela e Pedro", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	//Habilitar o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Habilitar a transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO_Background = setupGeometry();
	GLuint VAO_Player = setupGeometry();
	GLuint VAO_Meteoro = setupGeometry();
	GLuint VAO_Vidas = setupGeometry();

	// Geração da textura
	GLuint texID = generateTexture("../textures/space_bg.png");
	GLuint texIDPersonagem[4] = {
		generateTexture("../textures/naves/nave1.png"), 
		generateTexture("../textures/naves/nave2.png"),
		generateTexture("../textures/naves/nave3.png"),
		generateTexture("../textures/naves/nave4.png"),
	};
	GLuint texIDMeteoro = generateTexture("../textures/Meteor/Meteor1.png");
	GLuint texIDVidas [4] = {
		generateTexture("../textures/Vidas/0.png"),
		generateTexture("../textures/Vidas/1.png"),
		generateTexture("../textures/Vidas/2.png"),
		generateTexture("../textures/Vidas/3.png") 
	};


	// Criação da matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0f, 675.0f, 0.0f, 1000.0f, -1.0f, 1.0f);

	glUseProgram(shader.ID);
	
	shader.setMat4("projection", glm::value_ptr(projection));
	
	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);

	glActiveTexture(GL_TEXTURE0);

	// VIDAS
	int vidas = 3;

	// Posição inicial do background
	float bg_y = 600.0f;

	// Posição inicial do player
	float player_x = width/2;
	float player_y = 100.0; // SEMPRE O MESMO, PLAYER NÃO SE MEXE PARA CIMA

	// Velocidade do Foguete
	float velocidade = 0.5f;

	// Sorteia qual vai ser a skin do player da vez
	int skin = rand() % 4;

	// Inicialização dos vetores e variáveis de meteoros
	const int nro_obstaculos = 30;
	float pos_x_met[nro_obstaculos];
	float pos_y_met[nro_obstaculos];
	int meteoros_que_ja_colidiram[nro_obstaculos];
	fill_n(meteoros_que_ja_colidiram, nro_obstaculos, -1);
	int cont_met = 0;
	int pos_meteoro_mais_alto = 0;


	// Cálculo das posições dos meteoros
	for (size_t i = 0; i < nro_obstaculos; i++)
	{
		pos_x_met[i] = (50.0 + rand() % (WIDTH - 50));
		pos_y_met[i] = (1200.0 + rand() % (HEIGHT*20));

	}

	// Verificação da posição do meteoro mais alto
	// (para saber quando terminar o jogo se o player não morrer)
	for (size_t i = 0; i < nro_obstaculos; i++)
	{
		if (pos_y_met[pos_meteoro_mais_alto] < pos_y_met[i]) {
			pos_meteoro_mais_alto = i;
		}
	}

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{

		// Ouve os inputs do teclado e muda a posição do foguete de acordo
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && player_x > 50.f) {
			player_x -= velocidade;
		}

		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && player_x < (width - 50.f))
		{
			player_x += velocidade;
		}
		
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();
		
		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		/// IMAGEM DE BACKGROUND ------------------------------------------------------------------------------------------------------------------
		glm::mat4 model;		
		for (size_t i = 0; i < 3; i++)
		{
			drawObject(model, glm::vec3(337.5f, bg_y + (1200 * i), 0.0f), glm::vec3(675.0f, 1200.f, 1.0f), texID, VAO_Background, shader);
		}

		bg_y -= 0.25f;

		if (bg_y == -600) {
			bg_y = 600.0f;
		}

		/// IMAGEM DO FOGUETE ------------------------------------------------------------------------------------------------------------------
		drawObject(model, glm::vec3(player_x, player_y, 0.0f), glm::vec3(100.0f, 120.0f, 1.0f), texIDPersonagem[skin], VAO_Player, shader);

		/// IMAGEM DOS METEOROS ------------------------------------------------------------------------------------------------------------------
		for (size_t i = 0; i < nro_obstaculos; i++)
		{
			//Não renderiza os meteoros que já colidiram ou passaram 
			int teste_met = false;
			for (size_t j = 0; j < nro_obstaculos; j++)
			{
				if (meteoros_que_ja_colidiram[j] == i) {
					teste_met = true;
					continue;
				}
			}

							  // Só deixa renderizar os meteoros que estão aparecendo na tela, para não sobrecarregar	
			if (!teste_met && pos_y_met[i] < (height + 50)) {

				drawObject(model, glm::vec3(pos_x_met[i], pos_y_met[i], 0.0f), glm::vec3(50.0f, 50.0f, 1.0f), texIDMeteoro, VAO_Meteoro, shader);

				//Testa se o player colidiu com algum meteoro
				bool collisionX = pos_x_met[i] + 16.0 >= player_x - 50.0 && player_x + 50.0 >= pos_x_met[i] - 16.0;
				bool collisionY = pos_y_met[i] <= player_y + 50;
				if (collisionX && collisionY) {
					meteoros_que_ja_colidiram[cont_met] = i;
					cont_met++;
					vidas--;
				}

				// Apaga o meteoro que já desapareceu da tela
				if (pos_y_met[i] < -50) {
					meteoros_que_ja_colidiram[cont_met] = i;
					cont_met++;
				}

			}
			// Diminui a posição de cada meteoro
			pos_y_met[i] -= 0.5f;

		}
		/// IMAGEM DAS VIDAS -------------------------------------------------------------------------------------------------------------------
		drawObject(model, glm::vec3(120, height - 50, 0.0f), glm::vec3(203, 63, 1.0f), texIDVidas[vidas], VAO_Vidas, shader);


		//Desconectando o buffer
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);


		// Termina o jogo quando o último meteoro sai da tela ou se o player não tem mais vidas
		if (pos_y_met[pos_meteoro_mais_alto] == -1000.0 || vidas == 0) {
			break;
		}
	}

	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO_Background);
	glDeleteVertexArrays(1, &VAO_Player);
	glDeleteVertexArrays(1, &VAO_Meteoro);
	glDeleteVertexArrays(1, &VAO_Vidas);

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		//posição         //cor         //tex coord
		-0.5 , -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,  //v0
		 0.5,  -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,  //v1
		 0.5,  0.5 , 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,   //v2
		 //outro triangulo vai aqui
		-0.5 , -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,  //v0
		 0.5,   0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,   //v2
		-0.5,  0.5 , 0.0, 0.0, 0.0, 1.0, 0.0, 1.0,   //v3
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coord tex
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}

GLuint generateTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Parametriza o wrapping e o filtering da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	return texID;
}

void drawObject(glm::mat4 matriz_modelo, glm::vec3 translacao, glm::vec3 escala, GLuint idTextura, GLuint VAO, Shader shader)
{
	//Atualizando a matriz de modelo
	matriz_modelo = glm::mat4(1); //matriz de modelo: transformações na geometria		
	matriz_modelo = glm::translate(matriz_modelo, translacao);
	matriz_modelo = glm::scale(matriz_modelo, escala);
	shader.setMat4("model", glm::value_ptr(matriz_modelo));
	// Conectando com a textura a ser usada
	glBindTexture(GL_TEXTURE_2D, idTextura);
	//Passando o offset na textura para o shader 
	shader.setVec2("offset", 0.0, 0.0);
	// Chamada de desenho - drawcall
	glBindVertexArray(VAO); //conectando com o buffer de geometria correto
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
