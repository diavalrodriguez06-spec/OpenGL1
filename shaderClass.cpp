#include "shaderClass.h"

// Función para leer el contenido de un archivo y devolverlo como una cadena de texto
std :: string get_file_contents(const char* filename) {

	std::ifstream in(filename, std::ios::binary);
	if (in) {

		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;

	}
	throw(errno);
}
// Constructor que carga, compila y enlaza los shaders desde los archivos especificados
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	
	// Leer el código fuente de los shaders desde los archivos especificados
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convertir las cadenas de texto a punteros de caracteres para su uso en las funciones de OpenGL
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Crear y compilar el shader de vértices
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Enviar el código fuente del shader de vértices a OpenGL y compilarlo
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compilar el shader de vértices
	glCompileShader(vertexShader);
	// Verificar si hubo errores de compilación en el shader de vértices
	compileErrors(vertexShader, "VERTEX");

	// Crear y compilar el shader de fragmentos
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Enviar el código fuente del shader de fragmentos a OpenGL y compilarlo
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compilar el shader de fragmentos
	glCompileShader(fragmentShader);
	// Verificar si hubo errores de compilación en el shader de fragmentos
	compileErrors(fragmentShader, "FRAGMENT");

	// Crear un programa de shader y enlazar los shaders de vértices y fragmentos
	ID = glCreateProgram();
	// Enlazar el shader de vértices y el shader de fragmentos al programa de shader
	glAttachShader(ID, vertexShader);
	// Enlazar el shader de fragmentos al programa de shader
	glAttachShader(ID, fragmentShader);
	// Enlazar el programa de shader para que esté listo para su uso
	glLinkProgram(ID);
	// Verificar si hubo errores de enlace en el programa de shader
	compileErrors(ID, "PROGRAM");

	// Eliminar los shaders de vértices y fragmentos después de enlazarlos al programa de shader, ya que ya no son necesarios
	glDeleteShader(vertexShader);
	// Eliminar el shader de fragmentos después de enlazarlo al programa de shader, ya que ya no es necesario
	glDeleteShader(fragmentShader);
}
// Activar el programa de shader para su uso en el pipeline de renderizado
void Shader::Activate() {
	glUseProgram(ID);
}// Eliminar el programa de shader y liberar los recursos asociados
void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}