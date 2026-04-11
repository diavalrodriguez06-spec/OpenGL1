#include "EBO.h"

// Constructor que genera un Element Buffer Object (EBO), lo enlaza y carga los datos de los índices en la memoria de la GPU
EBO::EBO(GLuint* indices, GLsizeiptr size) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
// Enlazar el EBO para su uso en el pipeline de renderizado
void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
// Desenlazar el EBO para evitar modificaciones accidentales
void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
// Eliminar el EBO y liberar los recursos asociados
void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}
