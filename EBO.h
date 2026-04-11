#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO {
public:
	// ID del Element Buffer Object (EBO) que se utilizará para almacenar los índices de los vértices que forman los triángulos en la memoria de la GPU
	GLuint ID;
	// Constructor que genera un Element Buffer Object (EBO), lo enlaza y carga los datos de los índices en la memoria de la GPU
	EBO(GLuint* indices, GLsizeiptr size);

	// Enlazar el EBO para su uso en el pipeline de renderizado
	void Bind();
	// Desenlazar el EBO para evitar modificaciones accidentales
	void Unbind();
	// Eliminar el EBO y liberar los recursos asociados
	void Delete();
};

#endif
