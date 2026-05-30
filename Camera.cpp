#include "Camera.h"


Camera::Camera(int width, int height, glm::vec3 position) {
    Camera::width = width;
	Camera::height = height;
	this->position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
	
	// Crear las matrices de vista y proyección utilizando GLM
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// La matriz de vista se crea utilizando la función glm::lookAt, que toma la posición de la cámara, el punto al que está mirando (en este caso, la posición más la orientación) y el vector "up" para definir la dirección hacia arriba de la cámara
	view = glm::lookAt(position, position + Orientation, up);
	// La matriz de proyección se crea utilizando la función glm::perspective, que toma el campo de visión en grados, la relación de aspecto (ancho dividido por alto), y los planos cercano y lejano para definir el volumen de visión de la cámara
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	// Enviar la matriz combinada de proyección y vista al shader utilizando glUniformMatrix4fv
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));

}

void Camera::Inputs(GLFWwindow* window) {

	// Procesar las entradas del teclado para mover la cámara en el espacio 3D
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * Orientation;
	}
	// Para mover la cámara hacia los lados, se utiliza el producto cruzado entre la orientación de la cámara y el vector "up" para obtener un vector perpendicular que representa la dirección lateral. Luego, se normaliza este vector y se multiplica por la velocidad para mover la cámara en esa dirección.
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position += speed * -glm::normalize(glm::cross(Orientation, up));
	}
	// Para mover la cámara hacia adelante o hacia atrás, se multiplica la orientación de la cámara por la velocidad y se suma o resta a la posición de la cámara, respectivamente.
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position += speed * -Orientation;
	}
	// Para mover la cámara hacia los lados, se utiliza el producto cruzado entre la orientación de la cámara y el vector "up" para obtener un vector perpendicular que representa la dirección lateral. Luego, se normaliza este vector y se multiplica por la velocidad para mover la cámara en esa dirección.
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += speed * glm::normalize(glm::cross(Orientation, up));
	}
	// Para mover la cámara hacia arriba o hacia abajo, se multiplica el vector "up" por la velocidad y se suma o resta a la posición de la cámara, respectivamente.
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += speed * up;
	}
	// Para mover la cámara hacia arriba o hacia abajo, se multiplica el vector "up" por la velocidad y se suma o resta a la posición de la cámara, respectivamente.
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position += speed * -up;
	}
	// Para aumentar la velocidad de movimiento de la cámara, se verifica si la tecla Shift está presionada. Si lo está, se establece la velocidad a un valor más alto (0.4f en este caso). Si la tecla Shift no está presionada, se restablece la velocidad a su valor original (0.1f en este caso).
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.4f;
	}
	// Para aumentar la velocidad de movimiento de la cámara, se verifica si la tecla Shift está presionada. Si lo está, se establece la velocidad a un valor más alto (0.4f en este caso). Si la tecla Shift no está presionada, se restablece la velocidad a su valor original (0.1f en este caso).
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = 0.1f;
	}

	// Procesar las entradas del mouse para rotar la cámara
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		
		// Para ocultar el cursor del mouse cuando se presiona el botón izquierdo, se utiliza la función glfwSetInputMode con el modo GLFW_CURSOR y el valor GLFW_CURSOR_HIDDEN. Esto hace que el cursor del mouse desaparezca de la pantalla mientras se mantiene presionado el botón izquierdo, lo que es útil para evitar distracciones visuales mientras se rota la cámara.
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Para centrar el cursor del mouse en la ventana cuando se hace clic por primera vez, se verifica si la variable firstClick es verdadera. Si lo es, se utiliza la función glfwSetCursorPos para establecer la posición del cursor en el centro de la ventana (ancho dividido por 2 y alto dividido por 2). Después de centrar el cursor, se establece firstClick en falso para evitar que el cursor se vuelva a centrar en clics posteriores.
		if (firstClick) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// obtener nuevos offsets del ratón (igual que ahora)
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// calcular nueva orientación por pitch
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, up)));

		// permitir pitch salvo que esté demasiado cerca de los polos (evita flip)
		float angleWithUp = glm::angle(newOrientation, up);
		if (angleWithUp > glm::radians(5.0f) && angleWithUp < glm::radians(175.0f)) {
		    Orientation = newOrientation;
		}

		// aplicar yaw siempre
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), up);

		// re-centrar cursor
		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
 {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

}