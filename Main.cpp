#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Definir el código fuente del shader de vértices 
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Definir el código fuente del shader de fragmentos
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {
	// Inicializar GLFW.
	glfwInit();

	// Configurar GLFW para usar OpenGL 3.3 Core Profile
	// Esto asegura que estamos utilizando la versión correcta de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Esto indica que queremos usar el perfil core de OpenGL, lo que significa que no tendremos acceso a funciones obsoletas
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Definir los vértices de un triángulo equilátero centrado en el origen
	GLfloat vertices [] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Vértice inferior izquierdo
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Vértice inferior derecho
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Vértice superior
	};
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


	// Compilar el shader de vértices
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// El primer argumento es el tipo de shader (en este caso, un shader de vértices)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	
	// El segundo argumento es el número de cadenas de código fuente (en este caso, 1)
	glCompileShader(vertexShader);

	// Compilar el shader de fragmentos
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// El primer argumento es el tipo de shader (en este caso, un shader de fragmentos)
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);	
	// El segundo argumento es el número de cadenas de código fuente (en este caso, 1)
	glCompileShader(fragmentShader);

	// Crear un programa de shader y vincular los shaders compilados
	GLuint shaderProgram = glCreateProgram();
	// El primer argumento es el programa de shader al que se adjuntarán los shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Vincular el programa de shader para que pueda ser utilizado en el pipeline de renderizado
	glLinkProgram(shaderProgram);

	// Después de vincular el programa de shader, los shaders individuales ya no son necesarios
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Configurar los buffers de vértices y los arrays de vértices
	GLuint VAO, VBO;
	
	// Generar un Vertex Array Object (VAO) y un Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Vincular el VAO para que las siguientes llamadas a funciones de configuración de vértices se apliquen a este VAO
	glBindVertexArray(VAO);

	// Vincular el VBO como el buffer de vértices actual para el array de vértices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copiar los datos de los vértices al buffer de vértices en la GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	
	
	// Configurar los atributos de vértices para que OpenGL sepa cómo interpretar los datos de los vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// El primer argumento es el índice del atributo de vértices (en este caso, 0)
	glEnableVertexAttribArray(0);

	// Desvincular el VBO y el VAO para evitar modificaciones accidentales
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
		glUseProgram(shaderProgram);
		// Vincular el VAO para que las siguientes llamadas a funciones de dibujo se apliquen a este VAO
		glBindVertexArray(VAO);
		// Dibujar el triángulo utilizando los vértices configurados en el VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		// Aquí es donde se realizarían las operaciones de renderizado y actualización de la escena
		glfwPollEvents();
	}


	// Limpiar los recursos utilizados por OpenGL
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Limpiar y cerrar la aplicación
	glfwDestroyWindow(window);
	// Terminar GLFW para liberar los recursos utilizados por la biblioteca
	glfwTerminate();
	return 0;
	}
	