/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "Camera.hpp"

#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr

namespace Anim3D_Project2
{ 
	Camera::Camera(float fov, float near_z, float far_z, float ratio)
	{
		reset(fov, near_z, far_z, ratio);
	}

	//Función que permite calcular la rotación de la cámara a partir del desplazamiento del mouse por pantalla a partir de la variación en la posición del cursor
	glm::mat4 Camera::calculate_camera_rotation()
	{
		//Se suma al ángulo la distancia del mouse entre su posición inicial y la final
		camera_angleX += camera_deltaX;
		camera_angleY += camera_deltaY;

		//Limitamos la rotación vertical de la cámara para evitar invertir la vista al rotarse mas de 90 grados
		if      (camera_angleX < -1.5) { camera_angleX = -1.5;}
		else if (camera_angleX > +1.5) { camera_angleX = +1.5;}

		glm::mat4 camera_rotation;

		//Rotamos la cámara según dicha variación
		camera_rotation = glm::rotate(camera_rotation, glm::degrees(camera_angleY), glm::vec3(0.f, 1.f, 0.f));
		camera_rotation = glm::rotate(camera_rotation, glm::degrees(camera_angleX), glm::vec3(1.f, 0.f, 0.f));

		return camera_rotation;
	}

	//Devolvemos la cámara al inicio
	void Camera::reset(float new_fov, float new_near_z, float new_far_z, float new_ratio)
	{
		set_camera_fov  (new_fov   );
		set_camera_nearZ(new_near_z);
		set_camera_farZ (new_far_z );
		set_camera_aspect_ratio(new_ratio);
		set_location    (0.f, 0.f, 0.f );
		set_target      (0.f, 0.f, -1.f);
	}

	//Desplazamiento de la cámara en la escena
	void Camera::move(const glm::vec3 & translation)
	{
		camera_location += glm::vec4(translation, 1.f);
		camera_target   += glm::vec4(translation, 1.f);
	}

	//Rotación de la cámara en la escena
	void Camera::rotate(const glm::mat4 & rotation)
	{
		camera_target = camera_location + rotation * (camera_target - camera_location);
	}

	//Encapsulamiento que permite obtener la matriz de proyeccion
	glm::mat4 Camera::get_projection() const
	{
		return glm::perspective(camera_fov, camera_aspect_ratio, camera_nearZ, camera_farZ);
	}

	//Encapsulamiento que permite obtener la matriz de model view
	glm::mat4 Camera::get_model_view() const
	{
		return glm::lookAt
		(
			glm::vec3(camera_location[0], camera_location[1], camera_location[2]),
			glm::vec3(camera_target[0],   camera_target[1],   camera_target[2]  ),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
	}
}