#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
	// ID del Vertex Array Object (VAO) que se utilizará para almacenar la configuración de los vértices en la memoria de la GPU
	GLuint ID;
	// Constructor que genera un VAO
	VAO();
	
	// Enlazar un VBO al VAO y configurar la forma en que se interpretarán los datos de los vértices
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset);
	// Enlazar el VAO para su uso en el pipeline de renderizado
	void Bind();
	// Desenlazar el VAO para evitar modificaciones accidentales
	void Unbind();
	// Eliminar el VAO y liberar los recursos asociados
	void Delete();
};

#endif
