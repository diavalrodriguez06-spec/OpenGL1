#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera {
public:
	
	// Posición de la cámara en el espacio 3D
	glm::vec3 position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Variable para controlar el primer clic del mouse
	bool firstClick = true;

	// Matrices de vista y proyección
	int width;
	int height;

	// Velocidad de movimiento y sensibilidad del mouse para controlar la cámara
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Constructor para inicializar la cámara con el tamaño de la ventana y la posición inicial
	Camera(int width, int height, glm::vec3 position);

	// Función para calcular y enviar la matriz de vista y proyección al shader
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);

	
};
#endif
