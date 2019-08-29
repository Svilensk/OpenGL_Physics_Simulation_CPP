/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef VEHICLE_HEADER
#define VEHICLE_HEADER

#include "BulletWorldController.hpp"
#include "Model.hpp"

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

namespace Anim3D_Project2
{
	class Vehicle
	{
	private:
		//Posición del vehiculo, las ruedas, chasis y catapulta se instancian según este valor
		glm::vec3 vehiclePosition = glm::vec3(5.00f, -2.50f, -9.5f);

		//Time points para controlar la cadencia de disparo de la catapulta
		std::chrono::time_point<std::chrono::system_clock> m_StartTime;
		std::chrono::time_point<std::chrono::system_clock> m_NowTime;		
		
		//Fuerza de los motores que componen un vehiculo
		float catapultEngineForce = 15.f;
		float wheelEngineForce    = 1000.f;

	public:

		std::vector<btHinge2Constraint*> vehicle_Engines; //Vector con los 4 motores de las ruedas
		std::vector<Model*> vehicleDrawableModels; //Vector con todos los modelos del vehiculo
		btHingeConstraint*  catapult_Hinge;        //Puntero al eje de la catapulta (para modificarlo desde la escena)


		void spawn_sphere(); //Función que permite recolocar la esfera en la catapulta
		void check_disable_sphere(); //Comprobación para deshabilitar la esfera si ha caído fuera del mapa

		void set_drive_engine_vel(float velocity); //Setter de velocidad del motor de fuerza de las ruedas
		void set_steer_engine_vel(float velocity); //Setter de velocidad del motor direccional de las ruedas

		Vehicle(BulletWorldController* bulletWorldController);
	};
}
#endif // !VEHICLE_HEADER