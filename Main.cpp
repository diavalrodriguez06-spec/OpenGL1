#include<iostream>
#include<cmath>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// Definir los vértices de un triángulo equilátero centrado en el origen
GLfloat vertices[] = 
{ //          Posición                  /          Color           
	-0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f,   // Vértice inferior izquierdo
	 0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f,   // Vértice inferior derecho
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  1.0f, 0.6f,  0.032f,  // Vértice superior
   - 0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f,  // Vértice medio izquierdo
	 0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f,   // Vértice medio derecho
	 0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f  // Vértice inferior central
};
GLuint indices[] = {
	2, 3, 4, // Triángulo principal
	3, 0, 5, // Triángulo inferior izquierdo
	4, 5, 1  // Triángulo inferior derecho
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL,NULL);
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
	glViewport(0, 0, 800, 800);

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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0); // Posición
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	// Desenlazar el VAO, VBO y EBO para evitar modificaciones accidentales
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Obtener la ubicación del uniforme "scale" en el programa de shader para poder modificar su valor desde el código
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Configurar el color de fondo para la ventana
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	
	// Limpiar el buffer de color para aplicar el color de fondo configurado
	glClear(GL_COLOR_BUFFER_BIT);
	// Intercambiar los buffers de la ventana para mostrar el contenido renderizado
	glfwSwapBuffers(window);



	// Bucle principal de la aplicación
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Usar el programa de shader para renderizar la escena
		shaderProgram.Activate();
		// Modificar el valor del uniforme "scale" en el shader para controlar la escala de los objetos renderizados
		glUniform1f(uniID, 0.5f);
		// Vincular el VAO para que las siguientes llamadas a funciones de dibujo se apliquen a este VAO
		VAO1.Bind();
		// Dibujar el triángulo utilizando los vértices configurados en el VAO
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Aquí es donde se realizarían las operaciones de renderizado y actualización de la escena
		glfwPollEvents();
	}


	// Limpiar los recursos utilizados por OpenGL
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();


	// Limpiar y cerrar la aplicación
	glfwDestroyWindow(window);
	// Terminar GLFW para liberar los recursos utilizados por la biblioteca
	glfwTerminate();
	return 0;
	}
	