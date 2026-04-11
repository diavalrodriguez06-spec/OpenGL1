#include "VBO.h"

// Constructor que genera un VBO, lo enlaza y carga los datos de los vértices en la memoria de la GPU
VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
// Enlazar el VBO para su uso en el pipeline de renderizado
void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}
// Desenlazar el VBO para evitar modificaciones accidentales
void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// Eliminar el VBO y liberar los recursos asociados
void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}