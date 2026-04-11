
// Definir el código fuente del shader de fragmentos
#version 330 core
out vec4 FragColor;

// Entrada de color desde el shader de vértices
in vec3 color;	

void main()
{
   FragColor = vec4(color, 1.0f);
}