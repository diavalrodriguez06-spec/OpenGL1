#include<iostream>
#include<cmath>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;


// Cubo centrado en el origen (cada cara con 4 vértices -> 24 vértices)
GLfloat vertices[] = {
	// Posición         // Color         // Coordenadas de textura

	// Frontal (z = +0.5) - rojo
	-0.5f, -0.5f,  0.5f,  1.0f,0.0f,0.0f,   0.0f, 0.0f,// El primer vértice de la cara frontal tiene posición (-0.5, -0.5, 0.5), color rojo (1.0, 0.0, 0.0) y coordenadas de textura (0.0, 0.0)
     0.5f, -0.5f,  0.5f,  1.0f,0.0f,0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,0.0f,0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f,0.0f,0.0f,   0.0f, 1.0f,
	// Trasera (z = -0.5) - verde
     0.5f, -0.5f, -0.5f,  0.0f,1.0f,0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,1.0f,0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,1.0f,0.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,1.0f,0.0f,   0.0f, 1.0f,
	 // Trasera (z = -0.5) - verde
    -0.5f, -0.5f, -0.5f,  0.0f,0.0f,1.0f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,0.0f,1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,0.0f,1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,0.0f,1.0f,   0.0f, 1.0f,
	// Derecha (x = +0.5) - amarillo
     0.5f, -0.5f,  0.5f,  1.0f,1.0f,0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,1.0f,0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,1.0f,0.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,1.0f,0.0f,   0.0f, 1.0f,
	 // Superior (y = +0.5) - magenta
    -0.5f,  0.5f,  0.5f,  1.0f,0.0f,1.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,0.0f,1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,0.0f,1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f,0.0f,1.0f,   0.0f, 1.0f,
	// Inferior (y = -0.5) - cyan
    -0.5f, -0.5f, -0.5f,  0.0f,1.0f,1.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,1.0f,1.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,1.0f,1.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,1.0f,1.0f,   0.0f, 1.0f
};

GLuint indices[] = {
    //frontal
    0,1,2,  0,2,3,
    //trasera
    4,5,6,  4,6,7,
    // izquierda
    8,9,10, 8,10,11,
    // derecha
    12,13,14, 12,14,15,
    // superior
    16,17,18, 16,18,19,
    // inferior
    20,21,22, 20,22,23
};


int main() {
	// Inicializar GLFW.
	glfwInit();

	// Configurar GLFW para usar OpenGL 3.3 Core Profile
	// Esto asegura que estamos utilizando la versión correcta de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Esto indica que queremos usar el perfil core de OpenGL, lo que significa que no tendremos acceso a funciones obsoletas
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Crear una ventana de 800x800 píxeles con el título "LearnOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL,NULL);
	// Verificar si la ventana se creó correctamente
	if (window == NULL) {
		std::cout << "ERROR AL CREAR VENTANA" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Hacer que el contexto de OpenGL de la ventana sea el contexto actual para el hilo principal
	glfwMakeContextCurrent(window);

    // Cargar todas las funciones de OpenGL utilizando GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Establecer el tamaño del viewport
	// que es la región de la ventana donde se renderizará la escena
	// En este caso, estamos configurando el viewport para que ocupe toda la ventana de 800x800 píxeles
	glViewport(0, 0, width, height);

	// Crear un programa de shader utilizando los archivos de shader "default.vert" y "default.frag"
	Shader shaderProgram("default.vert", "default.frag");

	// Crear un Vertex Array Object (VAO) para almacenar la configuración de los vértices
	VAO VAO1;
	VAO1.Bind();

	// Crear un Vertex Buffer Object (VBO) para almacenar los datos de los vértices en la memoria de la GPU
	VBO VBO1(vertices, sizeof(vertices));
	// Crear un Element Buffer Object (EBO) para almacenar los índices de los vértices que forman los triángulos
	EBO EBO1(indices, sizeof(indices));

	// Configurar la forma en que se interpretarán los datos de los vértices
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // Posición
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Desenlazar el VAO, VBO y EBO para evitar modificaciones accidentales
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Obtener la ubicación del uniforme "scale" en el programa de shader para poder modificar su valor desde el código
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Textura
	Texture a("a.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	a.texUnit(shaderProgram, "tex0", 0);

	// Variable para controlar la rotación de los objetos en la escena
	float rotation = 0.0f;
	// Variable para almacenar el tiempo anterior y controlar la velocidad de rotación
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST); // Habilitar el test de profundidad para renderizar correctamente los objetos en 3D



	// Bucle principal de la aplicación
	while (!glfwWindowShouldClose(window)) {

		// Configurar el color de fondo para la ventana en cada iteración del bucle
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Limpiar el buffer de color para aplicar el color de fondo configurado en cada iteración del bucle
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Usar el programa de shader para renderizar la escena
		shaderProgram.Activate();

		double curntTime = glfwGetTime();
		if(curntTime - prevTime >= 1.0 /60.0) {
			rotation += 0.5f; // Incrementar la rotación en 1 grado cada 0.01 segundos
			prevTime = curntTime; // Actualizar el tiempo anterior para la próxima comparación
		}

		// Crear las matrices de transformación para el modelo, la vista y la proyección
		glm:: mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Aplicar una rotación al modelo en el eje Y utilizando la función glm::rotate
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(width/height),0.1f, 100.0f);

		// Obtener las ubicaciones de los uniformes "model", "view" y "proj" en el programa de shader y enviar las matrices de transformación correspondientes para que se apliquen a los objetos renderizados
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Modificar el valor del uniforme "scale" en el shader para controlar la escala de los objetos renderizados
		glUniform1f(uniID, 0.5f);
		// Vincular la textura para que se aplique a los objetos renderizados
		a.Bind();
		// Vincular el VAO para que las siguientes llamadas a funciones de dibujo se apliquen a este VAO
		VAO1.Bind();
		// Dibujar el triángulo utilizando los vértices configurados en el VAO
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Aquí es donde se realizarían las operaciones de renderizado y actualización de la escena
		glfwPollEvents();
	}


	// Limpiar los recursos utilizados por OpenGL
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	a.Delete();
	shaderProgram.Delete();


	// Limpiar y cerrar la aplicación
	glfwDestroyWindow(window);
	// Terminar GLFW para liberar los recursos utilizados por la biblioteca
	glfwTerminate();
	return 0;
	}