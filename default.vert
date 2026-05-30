// Vertex shader para renderizar un objeto con color y textura
#version 330 core

// Atributos de entrada
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// Salidas para el fragment shader
out vec3 color;
out vec2 texCoord;

// Matriz de camara (view * projection)
uniform mat4 camMatrix;

// Matriz de modelo (posicion y rotacion del objeto)
// Si no se usa, se puede dejar como identidad desde el CPU
uniform mat4 model;

void main()
{
// Transformamos la posición del vértice con la matriz de cámara y modelo
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
    color    = aColor;
    texCoord = aTexCoord;
}
