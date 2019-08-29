/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "Vehicle.hpp"

namespace Anim3D_Project2
{
	//Función que permite recolocar la esfera a la posición de la catapulta cuando esta va a disparar
	void Vehicle::spawn_sphere()
	{
		//Controlamos la cadencia de disparo
		m_NowTime = std::chrono::system_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(m_NowTime - m_StartTime).count() > 2000 || vehicleDrawableModels.back()->get_position().y < -7.f)
		{
			//Hacemos visible la esfera de nuevo
			vehicleDrawableModels.back()->is_visible = true;

			m_StartTime = std::chrono::system_clock::now();

			//Detenemos todas las fuerzas que podrían estar siendo aplicadas a la esfera
			vehicleDrawableModels.back()->body->clearForces();
			vehicleDrawableModels.back()->body->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
			vehicleDrawableModels.back()->body->setAngularVelocity(btVector3(0.f, 0.f, 0.f));

			//Obtenemos la posición de la catapulta donde debe aparecer la esfera, la convertimos a una matriz OpenGL
			//y ajustamos las coordenadas del transform en Y para que aparezca por encima, despues se lo devolvemos a Bullet
			//como un btTransform
			glm::mat4 opengl_spawn_pos;
			vehicleDrawableModels[5]->body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(opengl_spawn_pos));
			opengl_spawn_pos[3][0] = opengl_spawn_pos[3][0];
			opengl_spawn_pos[3][1] = opengl_spawn_pos[3][1] + 1.f;
			opengl_spawn_pos[3][2] = opengl_spawn_pos[3][2];

			btTransform bullet_pos;
			bullet_pos.setFromOpenGLMatrix(glm::value_ptr(opengl_spawn_pos));
			vehicleDrawableModels.back()->body->setWorldTransform(bullet_pos);
		}
	}

	//Función que permite hacer invisible la esfera si ha caído fuera del mapa
	//Ser invisible evita que se llame a su Update() y a su Render()
	void Vehicle::check_disable_sphere()
	{
		if (vehicleDrawableModels.back()->get_position().y < -10.f)
		{
			vehicleDrawableModels.back()->is_visible = false;
		}
	}

	//Configuración de la velocidad de aceleración de las ruedas
	void Vehicle::set_drive_engine_vel(float velocity)
	{
		vehicle_Engines[0]->setTargetVelocity(3, velocity);
		vehicle_Engines[1]->setTargetVelocity(3, velocity);
		vehicle_Engines[2]->setTargetVelocity(3, velocity);
		vehicle_Engines[3]->setTargetVelocity(3, velocity);
	}

	//Configuración del ángulo de dirección de las ruedas para el giro de la catapulta
	void Vehicle::set_steer_engine_vel(float velocity)
	{
		vehicle_Engines[0]->setTargetVelocity(5, velocity);
		vehicle_Engines[1]->setTargetVelocity(5, velocity);
		vehicle_Engines[2]->setTargetVelocity(5, -velocity);
		vehicle_Engines[3]->setTargetVelocity(5, -velocity);
	}

	Vehicle::Vehicle(BulletWorldController * bulletWorldController)
	{
		//Modelo [0] del vehiculo: Chasis
		vehicleDrawableModels.push_back(new Model(
			glm::vec3(vehiclePosition),
			glm::vec3(1.00f, 1.00f, 1.00f),
			glm::vec3(90.00f, 0.00f, 0.00f),
			"../../assets/models/Chassis.obj",
			"../../assets/textures/vehicle_tex.tga"));

		//Configuración física del chasis
		vehicleDrawableModels[0]->setup_body_physics(100, 2000.0f, 0.5f, btVector3(1.f, .2f, 1.f), btVector3(1, 1, 1), bulletWorldController);
		vehicleDrawableModels[0]->body->setAngularFactor(btVector3(1, 1, 1));
		vehicleDrawableModels[0]->body->setLinearFactor(btVector3(1, 1, 1));
		vehicleDrawableModels[0]->body->setActivationState(DISABLE_DEACTIVATION);

		//Modelo [1] del vehiculo: Rueda FL
		vehicleDrawableModels.push_back(new Model(
			glm::vec3(vehiclePosition.x - 1.f, vehiclePosition.y - 2.f, vehiclePosition.z - 1.f),
			glm::vec3(1.00f, 1.00f, 1.00f),
			glm::vec3(0.00f, 0.00f, 0.00f),
			"../../assets/models/Wheel.obj",
			"../../assets/textures/vehicle_tex.tga"));

		//Configuración física de la rueda FL
		vehicleDrawableModels[1]->setup_body_physics(101, 10.0f, 0.5f, .7f, btVector3(1, 1, 1), bulletWorldController);
		vehicleDrawableModels[1]->body->setAngularFactor(btVector3(1, 1, 1));
		vehicleDrawableModels[1]->body->setGravity      (btVector3(1, 1, 1));

		//Modelo [2] del vehiculo: Rueda FR
		vehicleDrawableModels.push_back(new Model(
			glm::vec3(vehiclePosition.x - 1.f, vehiclePosition.y - 2.f, vehiclePosition.z + 1.f),
			glm::vec3(1.00f, 1.00f, 1.00f),
			glm::vec3(0.00f, 0.00f, 0.00f),
			"../../assets/models/Wheel.obj",
			"../../assets/textures/vehicle_tex.tga"));

		//Configuración física de la rueda FR
		vehicleDrawableModels[2]->setup_body_physics(102, 10.0f, 0.5f, .7f, btVector3(1, 1, 1), bulletWorldController);
		vehicleDrawableModels[2]->body->setAngularFactor(btVector3(1, 1, 1));
		vehicleDrawableModels[2]->body->setGravity      (btVector3(1, 1, 1));

		//Modelo [3] del vehiculo: Rueda BL
		vehicleDrawableModels.push_back(new Model(
			glm::vec3(vehiclePosition.x + 1.f, vehiclePosition.y - 2.f, vehiclePosition.z - 1.f),
			glm::vec3(1.00f, 1.00f, 1.00f),
			glm::vec3(0.00f, 0.00f, 0.00f),
			"../../assets/models/Wheel.obj",
			"../../assets/textures/vehicle_tex.tga"));

		//Configuración física de la rueda BL
		vehicleDrawableModels[3]->setup_body_physics(103, 10.0f, 0.5f, .7f, btVector3(1, 1, 1), bulletWorldController);
		vehicleDrawableModels[3]->body->setAngularFactor(btVector3(1, 1, 1));
		vehicleDrawableModels[3]->body->setGravity      (btVector3(1, 1, 1));

		//Modelo [4] del vehiculo: Rueda BR
		vehicleDrawableModels.push_back(new Model(
			glm::vec3(vehiclePosition.x + 1.f, vehiclePosition.y - 2.f, vehiclePosition.z + 1.f),
			glm::vec3(1.00f, 1.00f, 1.00f),
			glm::vec3(0.00f, 0.00f, 0.00f),
			"../../assets/models/Wheel.obj",
			"../../assets/textures/vehicle_tex.tga"));

		//Configuración física de la rueda BR
		vehicleDrawableModels[4]->setup_body_physics(104, 10.0f, 0.5f, .7f, btVector3(1, 1, 1), bulletWorldController);
		vehicleDrawableModels[4]->body->setAngularFactor(btVector3(1, 1, 1));
		vehicleDrawableModels[4]->body->setGravity      (btVector3(1, 1, 1));

		//Modelo [5] del vehiculo: Catapulta
		vehicleDrawableModels.push_back(new Model(
			glm::vec3(vehiclePosition.x + 3.f, vehiclePosition.y - 0.5f, vehiclePosition.z),
			glm::vec3(1.20f, 1.20f, 1.20f),
			glm::vec3(90.00f, 180.f, -90.00f),
			"../../assets/models/Catapult.obj",
			"../../assets/textures/vehicle_tex.tga"));

		//Configuración física de la catapulta
		vehicleDrawableModels[5]->setup_body_physics(105, 10.f, 0.5f, btVector3(1.2f, .2f, .5f), btVector3(1, 1, 1), bulletWorldController);
		vehicleDrawableModels[5]->body->setAngularFactor(btVector3(1, 1, 1));
		vehicleDrawableModels[5]->body->setFriction(100.f);
		vehicleDrawableModels[5]->body->setActivationState(DISABLE_DEACTIVATION);

		//Modelo [6] del vehiculo: Esfera
		vehicleDrawableModels.push_back(new Model(
			glm::vec3(vehiclePosition    ),
			glm::vec3(0.50f, 0.50f, 0.50f),
			glm::vec3(0.00f, 0.00f, 0.00f),
			"../../assets/models/Planet2.obj",
			"../../assets/textures/vehicle_tex.tga"));

		//Configuración física de la esfera
		vehicleDrawableModels.back()->setup_body_physics(0, 2.f, 0.5f, .5f, btVector3(1, 1, 1), bulletWorldController);
		vehicleDrawableModels.back()->body->setAngularFactor(btVector3(1, 1, 1));

		vehicleDrawableModels.back()->body->setActivationState(DISABLE_DEACTIVATION);
		vehicleDrawableModels.back()->body->setFriction(1.f);
		vehicleDrawableModels.back()->is_visible = false;

		//Creamos la "Hinge" Catapulta, que unirá el brazo móvil con el chasis del vehiculo

		//Ajustamos los ejes de funcionamiento de la catapulta
		btVector3 childAxis(0.f, 0.f, 1.f);
		btVector3 parentAxis(0.f, 0.f, 1.f);

		//Creamos el eje de catapulta entre los dos bodies
		catapult_Hinge = new btHingeConstraint(*vehicleDrawableModels[0]->body, *vehicleDrawableModels[5]->body, btVector3(.5f, -.5f, 0), btVector3(-3.5f, 0, 0), parentAxis, childAxis);

		//Añadimos la catapulta al mundo dinámico
		bulletWorldController->dynamicsWorld->addConstraint(catapult_Hinge, true);

		//Configuramos las propiedades de la catapulta
		catapult_Hinge->enableMotor(true);
		catapult_Hinge->setMaxMotorImpulse(catapultEngineForce);
		catapult_Hinge->setMotorTargetVelocity(0);
		catapult_Hinge->setLimit(-2.f, .0f);

		//Creamos las uniones de cada una de las ruedas con el chasis del vehiculo
		//y las configuramos
		for (int i = 1; i <= 4; i++)
		{
			//Configuramos los parámetros de cada rueda
			vehicleDrawableModels[i]->body->setActivationState(DISABLE_DEACTIVATION);
			vehicleDrawableModels[i]->body->setFriction(1000.f);

			//Calculamos el punto de anclaje del hinge según la posición de cada rueda
			btTransform anchor_transform;
			anchor_transform.setIdentity();

			anchor_transform.setOrigin(btVector3(
				vehicleDrawableModels[i]->get_position().x,
				vehicleDrawableModels[i]->get_position().y,
				vehicleDrawableModels[i]->get_position().z));

			//Configuramos los ejes de funcionamiento y ajustamos el punto de anclaje
			btVector3 parentAxis(0.f, 1.f, 0.f);
			btVector3 childAxis(0.f, 0.f, 1.f);
			btVector3 anchor = anchor_transform.getOrigin();

			//Creamos la unión y la añadimos al vector de Engines(para poder interactuar con cada motor fuera de la clase)
			btHinge2Constraint* wheel_hinge = new btHinge2Constraint(*vehicleDrawableModels[0]->body, *vehicleDrawableModels[i]->body, anchor, parentAxis, childAxis);
			vehicle_Engines.push_back(wheel_hinge);

			//Añadimos la unión al mundo
			bulletWorldController->dynamicsWorld->addConstraint(wheel_hinge, true);

			//Configuración del motor de fuerza (Drive)
			wheel_hinge->enableMotor(3, true);
			wheel_hinge->setMaxMotorForce(3, wheelEngineForce);
			wheel_hinge->setTargetVelocity(3, 0);

			//Configuración del motor de dirección (Steer)
			wheel_hinge->enableMotor(5, true);
			wheel_hinge->setMaxMotorForce(5, wheelEngineForce);
			wheel_hinge->setTargetVelocity(5, 0);
			wheel_hinge->setDamping(2, 2.0);
			wheel_hinge->setStiffness(2, 4.0);
		}
	}
}