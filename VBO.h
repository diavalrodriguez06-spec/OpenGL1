#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO {
	public:
		// ID del Vertex Buffer Object (VBO) que se utilizará para almacenar los datos de los vértices en la memoria de la GPU
	GLuint ID;
	// Constructor que genera un VBO, lo enlaza y carga los datos de los vértices en la memoria de la GPU
	VBO(GLfloat* vertices, GLsizeiptr size);
	// Enlazar el VBO para su uso en el pipeline de renderizado
	void Bind();
	// Desenlazar el VBO para evitar modificaciones accidentales
	void Unbind();
	// Eliminar el VBO y liberar los recursos asociados	
	void Delete();
};

#endif
