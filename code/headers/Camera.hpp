/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

namespace Anim3D_Project2
{
	class Camera
	{
	private:

		float  camera_fov;           // Campo de visi�n
		float  camera_nearZ;         // Z cercana del frustrum
		float  camera_farZ;          // Z lejana del frustrum
		float  camera_aspect_ratio;  // Relaci�n de aspecto

		glm::vec4  camera_location;  // Posici�n en el espacio de la c�mara
		glm::vec4  camera_target;    // Target de la c�mara

	public:

		float  camera_angleX;  // �ngulo de rotaci�n en torno al eje X
		float  camera_angleY;  // �ngulo de rotaci�n en torno al eje Y
		float  camera_deltaX;  // Variaci�n de la rotaci�n en torno al eje X
		float  camera_deltaY;  // Variaci�n de la rotaci�n en torno al eje Y

		bool   mouse_press;    //Recepci�n de pulsaci�n del mouse en pantalla
		int    prev_mouseX;    //Localizaci�n del mouse en el frame previo ( c�lculo del delta )
		int    prev_mouseY;    //Localizaci�n del mouse en el frame previo ( c�lculo del delta )

		Camera(float fov = 60.f, float near_z = 0.1f, float far_z = 200.f, float ratio = 1.f);

		float get_fov()    const { return camera_fov;   }
		float get_near_z() const { return camera_nearZ; }
		float get_far_z()  const { return camera_farZ;  }
		float get_ratio()  const { return camera_aspect_ratio;  }

		const glm::vec4& get_location() const { return camera_location; } //Getter: posici�n de c�mara en el mundo
		const glm::vec4& get_target()   const { return camera_target;   } //Getter: target de c�mara en el mundo

		void set_camera_fov   (float _camera_fov  )       { camera_fov   = _camera_fov;   }
		void set_camera_nearZ (float _camera_nearZ)       { camera_nearZ = _camera_nearZ; }
		void set_camera_farZ  (float _camera_farZ )       { camera_farZ  = _camera_farZ;  }
		void set_location     (float x, float y, float z) { camera_location[0] = x; camera_location[1] = y; camera_location[2] = z; }
		void set_target       (float x, float y, float z) { camera_target[0] = x;   camera_target[1] = y;   camera_target[2] = z;   }
		void set_camera_aspect_ratio  (float _camera_aspect_ratio )  { camera_aspect_ratio  = _camera_aspect_ratio; }

		void reset (float new_fov, float new_near_z, float new_far_z, float new_ratio); //Reinicio de variables de la c�mara

		void move  (const glm::vec3 & translation); // desplazamiento de la c�mara a las coordenadas indicadas
		void rotate(const glm::mat4 & rotation);    // rotaci�n de la c�mera seg�n valores indicados

		glm::mat4 calculate_camera_rotation(); //C�lculo de la rotaci�n seg�n el mouse
		glm::mat4 get_projection() const; //Getter de la matriz de proyeccion
		glm::mat4 get_model_view() const; //Getter de la matriz model view
	};
}

#endif