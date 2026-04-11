#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
	public:
		// ID del programa de shader que se utilizará para renderizar la escena
	GLuint ID;
	// Constructor que carga, compila y enlaza los shaders desde los archivos especificados
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activar el programa de shader para su uso en el pipeline de renderizado
	void Activate();
	// Eliminar el programa de shader y liberar los recursos asociados
	void Delete();

private:
	// Función para verificar y mostrar los errores de compilación y enlace de los shaders
	void compileErrors(unsigned int shader, const char* type);
};




#endif // SHADER_CLASS_H
