#include "VAO.h"

// Constructor que genera un VAO
VAO::VAO() {
	glGenVertexArrays(1, &ID);
}
// Enlazar un VBO al VAO y configurar la forma en que se interpretarán los datos de los vértices
void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}
// Enlazar el VAO para su uso en el pipeline de renderizado
void VAO::Bind() {
	glBindVertexArray(ID);
}

// Desenlazar el VAO para evitar modificaciones accidentales
void VAO::Unbind() {
	glBindVertexArray(0);
}

// Eliminar el VAO y liberar los recursos asociados
void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}
