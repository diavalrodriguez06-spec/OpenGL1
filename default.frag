// Fragment shader para renderizar con textura y color
#version 330 core
// Salida del color final del fragmento
out vec4 FragColor;
// Entrada desde el vertex shader
in vec3 color;
in vec2 texCoord;

// Textura a muestrear
uniform sampler2D tex0;

// Función principal del fragment shader
void main()
{
// Muestra la textura en las coordenadas dadas
    vec4 texColor = texture(tex0, texCoord);
    // Mezcla textura con el color del vertice
    // Si no hay textura activa, se usa solo el color
    FragColor = texColor * vec4(color, 1.0);
}
