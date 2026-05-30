#include<iostream>
#include<cmath>
#include<vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"



const unsigned int width = 800;
const unsigned int height = 800;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 tex;
};

std::vector<Vertex> heartVertices;
std::vector<GLuint> heartIndices;


//  CORAZON 3D - capa por capa en Z
//  Cada capa es la silueta 2D del corazon escalada segun la profundidad
//  Esto preserva la forma de corazon vista de frente

void generarCorazon(int stepsU, int stepsZ) {
	// Limpiar datos previos
    heartVertices.clear();
    heartIndices.clear();

	// Parámetros del corazón
    const float PI = 3.14159265358979f;
    float scale = 0.07f;
    float profMax = 0.6f; // mitad del grosor total

    // Generar capas de Z (de -profMax a +profMax)
    for (int k = 0; k <= stepsZ; k++) {
        float z = -profMax + 2.0f * profMax * (float)k / stepsZ;
        // Factor de escala: en los bordes (z = +-profMax) el corazon es un punto
        // en el centro (z=0) es su tamaño maximo - forma de lente
        float factorZ = sqrtf(1.0f - (z / profMax) * (z / profMax));
		// Evitar que factorZ sea exactamente 0 en los bordes para no perder completamente la forma
        factorZ = glm::max(factorZ, 0.001f);

		// Generar la silueta del corazón en esta capa de Z
        for (int i = 0; i <= stepsU; i++) {
            float t = (float)i / stepsU * 2.0f * PI;

			// Ecuación paramétrica del corazón
            float sinT = sinf(t);
            float cosT = cosf(t);

            // Silueta del corazon en XY
            float x = 16.0f * sinT * sinT * sinT;
            float y = 13.0f * cosT - 5.0f * cosf(2 * t)
                - 2.0f * cosf(3 * t) - cosf(4 * t);

			// Crear el vértice con posición, color y coordenadas de textura
            Vertex vtx;
            vtx.pos = glm::vec3(x * scale * factorZ,
                y * scale * factorZ,
                z);
            vtx.color = glm::vec3(1.0f, 1.0f, 1.0f); // blanco -> textura sin teñir
            // UV: proyeccion planar desde el frente (la imagen se ve de frente sin distorsion)
            // x va de -16*scale a 16*scale, y va de -17*scale a 13*scale aprox
            float u_tex = (x * scale * factorZ) / (16.0f * scale) * 0.5f + 0.5f;
            
            float v_tex = (y * scale * factorZ) / (17.0f * scale) * 0.5f + 0.5f;
            vtx.tex = glm::vec2(u_tex, v_tex);
            heartVertices.push_back(vtx);
        }
    }

    // Indices conectando capas adyacentes
    for (int k = 0; k < stepsZ; k++) {
        for (int i = 0; i < stepsU; i++) {
            int a = k * (stepsU + 1) + i;
            int b = a + 1;
            int c = (k + 1) * (stepsU + 1) + i;
            int d = c + 1;
			// Dos triángulos por cada par de vértices adyacentes entre capas
            heartIndices.push_back(a); heartIndices.push_back(c); heartIndices.push_back(b);
            heartIndices.push_back(b); heartIndices.push_back(c); heartIndices.push_back(d);
        }
    }

    // Tapas: capa frontal y trasera - abanico desde el centro
    // Capa delantera (k = stepsZ)
    {
        int base = (int)heartVertices.size();
        // Centro de la tapa delantera
        Vertex centro;
		// En la capa delantera, el corazón es un punto en el centro (x=0,y=0) con z=profMax
        centro.pos = glm::vec3(0.0f, 0.0f, profMax);
        centro.color = glm::vec3(1.0f);
        centro.tex = glm::vec2(0.5f, 0.5f);
        heartVertices.push_back(centro);
        int centroIdx = base;

		// Conectar el centro con los vértices de la última capa para formar la tapa delantera
        int capaBase = stepsZ * (stepsU + 1);
        for (int i = 0; i < stepsU; i++) {
            heartIndices.push_back(centroIdx);
            heartIndices.push_back(capaBase + i);
            heartIndices.push_back(capaBase + i + 1);
        }
    }
    // Capa trasera (k = 0)
    {
        int base = (int)heartVertices.size();
        Vertex centro;
        centro.pos = glm::vec3(0.0f, 0.0f, -profMax);
        centro.color = glm::vec3(1.0f);
        centro.tex = glm::vec2(0.5f, 0.5f);
        heartVertices.push_back(centro);
        int centroIdx = base;

        for (int i = 0; i < stepsU; i++) {
            heartIndices.push_back(centroIdx);
            heartIndices.push_back(i + 1);
            heartIndices.push_back(i);
        }
    }
}


//  CURVA DE BEZIER CUBICA (4 puntos de control)

//  Para generar la curva, se itera t de 0 a 1 y se calcula B(t) para cada t
glm::vec3 P0(-4.0f, 0.0f, 4.0f);
glm::vec3 P1(-1.0f, 5.0f, 0.0f);
glm::vec3 P2(1.0f, 5.0f, 0.0f);
glm::vec3 P3(4.0f, 0.0f, -4.0f);

// Función para calcular un punto en la curva de Bezier dado un valor de t
glm::vec3 bezier(float t) {
    float u = 1.0f - t;
	return u * u * u * P0 + 3 * u * u * t * P1 + 3 * u * t * t * P2 + t * t * t * P3;// B(t) = (1-t)^3*P0 + 3(1-t)^2*t*P1 + 3(1-t)*t^2*P2 + t^3*P3
}
// Función para generar los vértices de la curva de Bezier, con color y coordenadas de textura
std::vector<float> generarLineaBezier(int N) {
    std::vector<float> v;
    for (int i = 0; i <= N; i++) {
        float t = (float)i / N;
        glm::vec3 p = bezier(t);
        v.push_back(p.x); v.push_back(p.y); v.push_back(p.z);
        v.push_back(0.0f); v.push_back(1.0f); v.push_back(1.0f);
        v.push_back(0.0f); v.push_back(0.0f);
    }
    return v;
}

// Puntos de control (solo para referencia, se pueden ocultar)
std::vector<float> generarPuntosControl() {
    std::vector<float> v;
    glm::vec3 pts[] = { P0, P1, P2, P3 };
    for (auto& p : pts) {
        v.push_back(p.x); v.push_back(p.y); v.push_back(p.z);
        v.push_back(1.0f); v.push_back(0.5f); v.push_back(0.0f);
        v.push_back(0.0f); v.push_back(0.0f);
    }
    return v;
}

int main() {
    glfwInit();
	// Configuración de la ventana y contexto OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Crear la ventana
    GLFWwindow* window = glfwCreateWindow(width, height, "Corazon en Bezier", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, width, height);

	// Cargar el shader
    Shader shader("default.vert", "default.frag");

	// Cargar la textura y asignar la unidad de textura al shader
    Texture textura("a.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    textura.texUnit(shader, "tex0", 0);

	// Generar el modelo del corazón y preparar los buffers de OpenGL
    generarCorazon(100, 30);
	// Flatten de los datos del corazón para subir a OpenGL (posiciones, colores, UVs)
    std::vector<float> heartFlat;
    for (auto& v : heartVertices) {
		heartFlat.push_back(v.pos.x); heartFlat.push_back(v.pos.y); heartFlat.push_back(v.pos.z);// posiciones
		heartFlat.push_back(v.color.r); heartFlat.push_back(v.color.g); heartFlat.push_back(v.color.b);// colores
		heartFlat.push_back(v.tex.x); heartFlat.push_back(v.tex.y);// coordenadas de textura
    }

	// Crear VAO, VBO y EBO para el corazón
    GLuint VAO_heart, VBO_heart, EBO_heart;
	glGenVertexArrays(1, &VAO_heart); glGenBuffers(1, &VBO_heart); glGenBuffers(1, &EBO_heart);// generar los IDs de los buffers
	glBindVertexArray(VAO_heart);// vincular el VAO para configurar los atributos de vértice
	glBindBuffer(GL_ARRAY_BUFFER, VBO_heart);// vincular el VBO para subir los datos de vértice
	glBufferData(GL_ARRAY_BUFFER, heartFlat.size() * sizeof(float), heartFlat.data(), GL_STATIC_DRAW);// subir los datos de vértice al VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_heart);// vincular el EBO para subir los índices de los triángulos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, heartIndices.size() * sizeof(GLuint), heartIndices.data(), GL_STATIC_DRAW);// subir los índices al EBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);          glEnableVertexAttribArray(0);// posición: 3 floats, stride de 8 floats (pos+color+tex), offset 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);// color: 3 floats, stride de 8 floats, offset de 3 floats (después de la posición)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);// coordenadas de textura: 2 floats, stride de 8 floats, offset de 6 floats (después de la posición y el color)
    glBindVertexArray(0);

	// Generar la curva de Bezier y preparar los buffers de OpenGL
	const int N_SEG = 120;// número de segmentos para aproximar la curva de Bezier
	std::vector<float> lineaBez = generarLineaBezier(N_SEG);// generar los vértices de la curva de Bezier (posiciones, colores, UVs)
	GLuint VAO_bez, VBO_bez;// no necesitamos EBO para la curva, se dibuja con GL_LINE_STRIP
	glGenVertexArrays(1, &VAO_bez); glGenBuffers(1, &VBO_bez);// generar los IDs de los buffers para la curva de Bezier
	glBindVertexArray(VAO_bez);// vincular el VAO para configurar los atributos de vértice de la curva de Bezier
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bez);// vincular el VBO para subir los datos de vértice de la curva de Bezier
	// Subir los datos de vértice de la curva de Bezier al VBO (posiciones, colores, UVs)
    glBufferData(GL_ARRAY_BUFFER, lineaBez.size() * sizeof(float), lineaBez.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);          glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glBindVertexArray(0);

	// Generar los puntos de control de la curva de Bezier (opcional, para referencia) y preparar los buffers de OpenGL
    std::vector<float> ctrlPts = generarPuntosControl();
    GLuint VAO_ctrl, VBO_ctrl;
    glGenVertexArrays(1, &VAO_ctrl); glGenBuffers(1, &VBO_ctrl);
    glBindVertexArray(VAO_ctrl);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ctrl);
    glBufferData(GL_ARRAY_BUFFER, ctrlPts.size() * sizeof(float), ctrlPts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);          glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

	// Crear la cámara y configurar su posición inicial
    Camera camera(width, height, glm::vec3(0.0f, 4.0f, 10.0f));

	// Variables para controlar la animación del corazón a lo largo de la curva de Bezier
	bool  modoAnimacion = false;// false = modo manual (corazón fijo en el centro), true = modo animación (corazón se mueve a lo largo de la curva de Bezier)
	float bezierT = 0.0f;// parámetro t para la curva de Bezier, va de 0.0f a 1.0f
	float bezierSpeed = 0.2f;// velocidad de animación a lo largo de la curva de Bezier (t por segundo)
	bool  teclaB = false;// para detectar el momento en que se presiona la tecla B (toggle de animación)
    float lastTime = (float)glfwGetTime();

	std::cout << "B    -> activar/desactivar animacion" << std::endl;// al activar la animación, el corazón se mueve a lo largo de la curva de Bezier, recorriéndola continuamente
	std::cout << "WASD -> mover camara" << std::endl;// controles de cámara: W = adelante, S = atrás, A = izquierda, D = derecha
	std::cout << "ESC  -> salir" << std::endl;// presionar ESC para cerrar la ventana y salir del programa

	// Bucle principal de renderizado
    while (!glfwWindowShouldClose(window)) {
		float now = (float)glfwGetTime();// tiempo actual en segundos desde que se inició el programa
		float dt = now - lastTime;// delta time: tiempo transcurrido desde el último frame, se usa para hacer que la animación sea independiente de la velocidad de fotogramas
		lastTime = now;// actualizar el tiempo del último frame

		// Limpiar la pantalla y activar el shader
		glClearColor(0.01f, 0.07f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Activate();

		// Detectar si se presiona la tecla B para activar/desactivar la animación del corazón a lo largo de la curva de Bezier
        bool B_ahora = glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;
        if (B_ahora && !teclaB) {
            modoAnimacion = !modoAnimacion;
            if (modoAnimacion) { bezierT = 0.0f; std::cout << "[BEZIER ON]" << std::endl; }
            else                std::cout << "[MANUAL ON]" << std::endl;
        }
		teclaB = B_ahora;// actualizar el estado de la tecla B para detectar el momento en que se presiona

		// Calcular la posición del corazón: si estamos en modo animación, se actualiza el parámetro t para recorrer la curva de Bezier y se calcula la posición del corazón en la curva; si estamos en modo manual, el corazón se mantiene fijo en el centro (0,0,0)
        glm::vec3 posCorazon(0.0f);
		// Si el modo de animación está activo, actualizar el parámetro t para recorrer la curva de Bezier y calcular la posición del corazón en la curva
        if (modoAnimacion) {
            bezierT += bezierSpeed * dt;
            if (bezierT > 1.0f) bezierT = 0.0f;
            posCorazon = bezier(bezierT);
        }
		camera.Inputs(window);// actualizar la cámara con los inputs del usuario (WASD para mover la cámara)

		// Calcular las matrices de vista y proyección de la cámara, y la matriz de modelo para el corazón (que incluye la posición en la curva de Bezier y una rotación sobre el eje Y para darle un efecto de giro)
        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.Orientation, camera.up);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
        glm::mat4 camMatrix = proj * view;

        // Corazon: trasladar a posicion en la curva + rotar sobre Y
        float angle = (float)glfwGetTime() * 1.2f;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), posCorazon);
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		// Enviar las matrices al shader
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Dibujar el corazón con textura
        textura.Bind();
        glBindVertexArray(VAO_heart);
        glDrawElements(GL_TRIANGLES, (GLsizei)heartIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Curva y puntos sin textura
        glBindTexture(GL_TEXTURE_2D, 0);
        glm::mat4 identity = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(identity));

		// Curva de Bezier
		glLineWidth(3.0f);// aumentar el grosor de la línea para que se vea mejor
		glBindVertexArray(VAO_bez);// vincular el VAO de la curva de Bezier para dibujarla
		glDrawArrays(GL_LINE_STRIP, 0, N_SEG + 1);// dibujar la curva de Bezier como una línea continua (GL_LINE_STRIP) usando los vértices del VAO_bez
		glBindVertexArray(0);// desvincular el VAO después de dibujar la curva de Bezier

 
		// Puntos de control de la curva de Bezier (opcional, para referencia)
        glfwSwapBuffers(window);
        glfwPollEvents();

		// Detectar si se presiona la tecla ESC para cerrar la ventana y salir del programa
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

	// Limpiar los recursos de OpenGL y cerrar la ventana antes de salir del programa
    glDeleteVertexArrays(1, &VAO_heart); glDeleteBuffers(1, &VBO_heart); glDeleteBuffers(1, &EBO_heart);
    glDeleteVertexArrays(1, &VAO_bez);   glDeleteBuffers(1, &VBO_bez);
    glDeleteVertexArrays(1, &VAO_ctrl);  glDeleteBuffers(1, &VBO_ctrl);
    textura.Delete();
    shader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}