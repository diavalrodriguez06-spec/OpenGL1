// Definir el código fuente del shader de vértices
#version 330 core
// Entrada de posición 
layout (location = 0) in vec3 aPos;
// Entrada de color
layout (location = 1) in vec3 aColor;
// Entrada de coordenadas de textura
layout (location = 2) in vec2 aTexCoord;

// Salida del color al shader de fragmentos
out vec3 color;

// Salida de las coordenadas de textura al shader de fragmentos
out vec2 texCoord;

uniform float scale;

// Matriz de modelo para transformar las posiciones de los vértices
uniform mat4 model;
// Matriz de vista para transformar las posiciones de los vértices
uniform mat4 view;
// Matriz de proyección para transformar las posiciones de los vértices
uniform mat4 proj;

void main()
{
// Transformar la posición del vértice usando las matrices de modelo, vista y proyección
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    // Pasar el color al shader de fragmentos
    color = aColor;
    // Pasar las coordenadas de textura al shader de fragmentos
    texCoord = aTexCoord;
}
