
// Definir el código fuente del shader de fragmentos
#version 330 core
out vec4 FragColor;

// Entrada de color desde el shader de vértices
in vec3 color;	

// Entrada de coordenadas de textura desde el shader de vértices
in vec2 texCoord;

// Uniforme para la textura
uniform sampler2D tex0;

void main()
{
   FragColor = texture(tex0, texCoord);
}