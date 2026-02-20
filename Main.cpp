#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

int main() {
	// Inicializar GLFW
	glfwInit();

	// Configurar GLFW para usar OpenGL 3.3 Core Profile
	// Esto asegura que estamos utilizando la versión correcta de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Esto indica que queremos usar el perfil core de OpenGL, lo que significa que no tendremos acceso a funciones obsoletas
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Crear una ventana de 800x600 píxeles con el título "LearnOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL,NULL);
	// Verificar si la ventana se creó correctamente
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Hacer que el contexto de OpenGL de la ventana sea el contexto actual para el hilo principal
	glfwMakeContextCurrent(window);

	// Cargar todas las funciones de OpenGL utilizando GLAD
	gladLoadGL();

	// Establecer el tamaño del viewport
	// que es la región de la ventana donde se renderizará la escena
	// En este caso, estamos configurando el viewport para que ocupe toda la ventana de 800x600 píxeles
	glViewport(0, 0, 800, 600);

	// Configurar el color de fondo para la ventana
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	
	// Limpiar el buffer de color para aplicar el color de fondo configurado
	glClear(GL_COLOR_BUFFER_BIT);
	// Intercambiar los buffers de la ventana para mostrar el contenido renderizado
	glfwSwapBuffers(window);

	// Bucle principal de la aplicación
	while (!glfwWindowShouldClose(window)) {
		// Aquí es donde se realizarían las operaciones de renderizado y actualización de la escena
		glfwPollEvents();
	}


	// Limpiar y cerrar la aplicación
	glfwDestroyWindow(window);
	// Terminar GLFW para liberar los recursos utilizados por la biblioteca
	glfwTerminate();
	return 0;
	}
	