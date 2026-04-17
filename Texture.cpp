#include "Texture.h"
#include <stb/stb_image.h>
#include <glad/glad.h>

// Constructor para cargar una imagen y crear una textura OpenGL
Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
   
	// Guardar el tipo de textura para su uso posterior
    type = texType;

    // Forzar 4 canales y voltear verticalmente si lo deseas
    stbi_set_flip_vertically_on_load(true);
	// Cargar la imagen utilizando stb_image, forzando a RGBA (4 canales)
    int width, height, nrChannels;
    unsigned char* data = stbi_load(image, &width, &height, &nrChannels, 4); // forzar RGBA

	// Verificar si la imagen se cargó correctamente
    if (!data) {
        std::cerr << "ERROR: no se pudo cargar la imagen: " << image << std::endl;
        ID = 0;
        return;
    }

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + 0); // la unidad real será seleccionada en texUnit
    glBindTexture(GL_TEXTURE_2D, ID);

    // Alineación para bytes no alineados
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Subir la textura (ahora datos tienen 4 canales, usar GL_RGBA)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Opcional: comprobar errores en desarrollo
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "GL error tras crear textura: " << err << std::endl;
    }
}

// Asignar la unidad de textura al shader
void Texture::texUnit(Shader& shader, const char* uniform, unsigned int unit) {
    shader.Activate();
    glUniform1i(glGetUniformLocation(shader.ID, uniform), unit);
}

// Enlazar la textura para su uso en el pipeline de renderizado
void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(type, ID);
}

// Desenlazar la textura para evitar modificaciones accidentales
void Texture::Unbind() {
    glBindTexture(type, 0);
}

// Eliminar la textura y liberar los recursos asociados 
void Texture::Delete() {
    glDeleteTextures(1, &ID);
}
