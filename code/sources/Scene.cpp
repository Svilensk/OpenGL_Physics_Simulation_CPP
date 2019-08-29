/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "Scene.hpp"
#include <iostream>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <btBulletDynamicsCommon.h>

using namespace std;
namespace Anim3D_Project2
{
	//El constructor de escena requiere de un Skybox y un postprocesado unico
	Scene::Scene(int width, int height) :
		skybox("../../assets/textures/miramar-"),
		postprocess("../../assets/textures/PlainOrange_PostProcess_texture.tga"),
		bulletWorldController(),
		playerVehicle(&bulletWorldController)
		{

		//Creo los modelos de la escena y los añado vector de modelos a dibujar.
		//El constructor de estos modelos recibe tanto una posición, rotación y translación inicial,
		//así como un modelo 3d y una textura opcional.

		//[0]PLATAFORMA INICIAL
		Scene::drawableModels.push_back(new Model(
			glm::vec3( 5.00f, -5.00f, -10.0f ), 
			glm::vec3( 1.00f,  1.00f,  1.00f ), 
			glm::vec3( 90.0f,  0.00f,  90.0f ),
			"../../assets/models/Platform.obj",
			"../../assets/textures/Platform_tex.tga"));

			//Parámetros: User Id / Masa / Factor de restitución / Tamaño (bien un vec3 para boxes o un float para esferas) / Factor lineal para los constraints por eje / referencia a worldcontroller
			drawableModels[0]->setup_body_physics(0, 0.f, 0.5f, btVector3(4.0f, .2f, 6.00f), btVector3(0, 0, 0), &bulletWorldController);

		//[1]PLATAFORMA CENTRAL
		Scene::drawableModels.push_back(new Model(
			glm::vec3(-15.0f, -5.00f, -10.0f ),
			glm::vec3( 1.00f,  1.00f,  1.00f ), 
			glm::vec3( 90.0f,  0.00f,  90.0f ),
			"../../assets/models/Central_Platform.obj",
			"../../assets/textures/Platform_tex.tga"));

			drawableModels[1]->setup_body_physics(1, 0.f, 0.5f, btVector3(8.0f, .2f, 6.0f), btVector3(0, 0, 0), &bulletWorldController);

		//[2]PLATAFORMA FINAL
		Scene::drawableModels.push_back(new Model(
			glm::vec3(-35.0f, -5.00f, -10.0f ), 
			glm::vec3( 1.00f,  1.00f,  1.00f ), 
			glm::vec3( 90.0f,  0.00f, 90.00f ),
			"../../assets/models/Platform.obj",
			"../../assets/textures/Platform_tex.tga"));

			drawableModels[2]->setup_body_physics(2, 0.f, 0.5f, btVector3(4.0f, .2f, 6.00f), btVector3(0, 0, 0), &bulletWorldController);

		//[3]PLATAFORMA MÓVIL
		Scene::drawableModels.push_back(new Model(
			glm::vec3(-5.20f, -5.00f, -10.0f ),
			glm::vec3( 1.00f,  1.00f,  1.00f ), 
			glm::vec3( 90.0f,  0.00f,  0.00f ),
			"../../assets/models/Movable_platform.obj",
			"../../assets/textures/Platform_tex.tga"));

			drawableModels[3]->setup_body_physics(3, 500000.f, 0.5f, btVector3(1.7f, .2f, 2.00f), btVector3(1, 0, 0), &bulletWorldController);
			drawableModels[3]->body->setActivationState(DISABLE_DEACTIVATION);


		//[4]OBJETIVO A DERRIBAR
		Scene::drawableModels.push_back(new Model(
			glm::vec3(-35.0f, -2.10f, -10.0f ),
			glm::vec3( 1.00f,  1.00f,  1.00f ), 
			glm::vec3(-90.0f,  0.00f, 90.00f ),
			"../../assets/models/Target.obj"  ,
			"../../assets/textures/Target_tex.tga"));

			drawableModels[4]->setup_body_physics(4, 5.f, 0.5f, btVector3(0.2f, 3.0f, 2.00f), btVector3(1, 1, 1), &bulletWorldController);
			drawableModels[4]->body->setAngularFactor(btVector3(1, 1, 1));
			drawableModels[4]->body->setActivationState(DISABLE_DEACTIVATION);

		//[5]MARCO DE PUERTA
		Scene::drawableModels.push_back(new Model(
			glm::vec3(-15.0f, -2.50f, -10.2f ),
			glm::vec3( 1.00f,  1.00f,  1.00f ),
			glm::vec3(-90.0f,  0.00f,  90.0f ),
			"../../assets/models/Door_Border.obj"));

			//La cofiguración física del marco de la puerta es ligeramente mas compleja, ya que se conforma de múltiples
			//shapes físicas, estas shapes se añaden a una btCompoundShape como hijos para mas adelante pasarse al modelo
		    //del marco de la puerta

			//Creamos la forma compuesta
			shared_ptr< btCompoundShape> compound(new btCompoundShape());
			bulletWorldController.collisionShapes.push_back(compound);

			//Creamos la forma hija, una btBoxShape para las columnas que forman el marco
			shared_ptr<btCollisionShape> door_support(new btBoxShape(btVector3(.2f, 5.0f, .5f)));
			bulletWorldController.collisionShapes.push_back(door_support);

			//btTransform con la posición local relativa de las shapes hijas
			btTransform localTrans;
			localTrans.setIdentity();

			//Situamos el modelo en el espacio y añadimos la forma como hija
			localTrans.setOrigin(btVector3(0, 0, -6));
			compound->addChildShape(localTrans, &*door_support);
			localTrans.setOrigin(btVector3(0, 0, 6));
			compound->addChildShape(localTrans, &*door_support);

			//Configuramos la física del modelo con la nueva forma compuesta y todos sus hijos
			drawableModels[5]->setup_body_physics(compound, 5, 0.0f, 0.5f, btVector3(1, 1, 1), &bulletWorldController);

		//[6]LLAVE
		Scene::drawableModels.push_back(new Model(
			glm::vec3( -10.50f, -3.50f, -12.5f), 
			glm::vec3( 1.00f,  1.00f,  1.00f), 
			glm::vec3(-90.0f,  0.00f,  90.0f),
			"../../assets/models/Key.obj",
			"../../assets/textures/key.tga"));

			drawableModels[6]->setup_body_physics(6, 0.0f, 0.5f, btVector3(0.2f, 0.2f, 0.2f), btVector3(0, 0, 0), &bulletWorldController);

		//[7]PUERTA
		Scene::drawableModels.push_back(new Model(
			glm::vec3(-15.00f, -2.50f, -10.2f ),
			glm::vec3(  1.00f,  1.00f,  1.00f ),
			glm::vec3( -90.0f,  0.00f,  90.0f ),
			"../../assets/models/Door.obj",
			"../../assets/textures/barrier.tga"));

			drawableModels[7]->setup_body_physics(7, 0.0f, 0.5f, btVector3(0.2f, 3.0f, 6.00f), btVector3(0, 0, 0), &bulletWorldController);
			drawableModels[7]->set_transparency(0.75f);

		resize(width, height);
	}

	void Scene::update()
	{			
		//Actualizamos la física del mundo cada update
		bulletWorldController.physics_step(60.f);	

		//Booleano que pasa a true si el jugador choca("coge") la llave para abrir la puerta
		bool key_colision = false;

		//Comprobamos si el chasis o alguna de las ruedas del vehiculo ha colisionado con la llave
		if (bulletWorldController.check_collisions(6, 100) ||
			bulletWorldController.check_collisions(6, 101) ||
			bulletWorldController.check_collisions(6, 102) ||
			bulletWorldController.check_collisions(6, 103) ||
			bulletWorldController.check_collisions(6, 104)) key_colision = true;


		//Al colisionar con la llave si la puerta no ha sido destruida, eliminamos los cuerpos físicos de puerta y llave
		//así como los modelos de los mismos
		if (key_colision && !door_destroyed )
		{
			drawableModels[7]->destroy( &bulletWorldController);
			drawableModels[6]->destroy( &bulletWorldController);

			drawableModels.erase(drawableModels.begin() + 7);
			drawableModels.erase(drawableModels.begin() + 6);

			door_destroyed = true;
		}

		//Si la puerta ( y llave ) no han sido destruidas, se anima la llave para que rote sobre si misma
		if (!door_destroyed){
			accumulated_rotation1 += 0.5;
			drawableModels[6]->set_rotation(glm::vec3(drawableModels[6]->get_rotation().x, drawableModels[6]->get_rotation().y, accumulated_rotation1));
		}

		//Desplazamiento de la plataforma móvil (Si llega a X >= .75f, el desplazamiento es x .75f
		if(drawableModels[3]->get_position().x >= -0.75f){
		   drawableModels[3]->body->setLinearVelocity(btVector3(-.5f, 0.0f, 0.0f));		
		}

		//Desplazamiento de la plataforma móvil
		else 
		if (drawableModels[3]->get_position().x <= -5.20f){
			drawableModels[3]->body->setLinearVelocity(btVector3(.5f, 0.0f, 0.0f));		
		}

		//Actualizamos cada modelo del vehiculo
		for (auto& vehicleparts : playerVehicle.vehicleDrawableModels)
		{
			if (vehicleparts->is_visible)
			vehicleparts->update();
		}

		//Actualizamos cada modelo de la escena
		for (auto& model : drawableModels)
		{
			if (model->is_visible)
			model->update();
		}

		//Actualizo las propiedades de la cámara cada update
		camera.set_target  (playerVehicle.vehicleDrawableModels[0]->get_position().x - 5.f, playerVehicle.vehicleDrawableModels[0]->get_position().y, playerVehicle.vehicleDrawableModels[0]->get_position().z);
		camera.set_location(playerVehicle.vehicleDrawableModels[0]->get_position().x, 2.f, 5.f);

		//Actualizo los valores de input
		update_input();

		//Comprobación si se debe dejar de renderizar la esfera, ya que ha caído fuera del escenario
		playerVehicle.check_disable_sphere();
	}

	void Scene::render()
	{
		//Limpio tanto el buffer de bits como el buffer de Z
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Renderizo el skybox
		skybox.render(camera);

		//Recorro todos los render de modelo dentro del vehiculo del jugador
		for (auto& vehicleparts : playerVehicle.vehicleDrawableModels)
		{
			if (vehicleparts->is_visible)
			vehicleparts->render(camera);
		}

		//Recorro los renderers de cada modelo dentro de drawableModels
		for (auto &model : drawableModels)
		{
			if(model->is_visible)
			model->render(camera);
		}

		//Renderizo el postproceso
		postprocess.render(camera);
	}

	void Scene::resize(int _width, int _height)
	{
		//Ajusto la nueva projection
		scene_width  = _width;
		scene_height = _height;
		camera.set_camera_aspect_ratio(float(scene_width) / scene_height);

		glViewport(0, 0, scene_width, scene_height);
	}

	//Según el input recibido ajustamos los booleanos relativos a cada tecla
	//Ajustar mediante booleanos las teclas pulsadas permite que facilmente se puedan recibir
	//Múltiples Inputs simultáneos.
	void Scene::on_key_pressed(int key_code)
	{
		switch (key_code)
		{
			case sf::Keyboard::W:	
				w_pressed = true;
				break;

			case sf::Keyboard::A:
				a_pressed = true;
				break;

			case sf::Keyboard::S:
				s_pressed = true;
				break;

			case sf::Keyboard::D:
				d_pressed = true;
				break;
			case sf::Keyboard::Space:
				playerVehicle.spawn_sphere();
				catapult_pressed = true;
				break;
		}
	}

	//Si se suelta la tecla de desplazamiento, se ajusta el booleano asociado a false
	void Scene::on_key_release(int key_code)
	{ 
		switch (key_code)
		{
			case sf::Keyboard::W:
				w_pressed = false;
				break;

			case sf::Keyboard::A:
				a_pressed = false;
				break;

			case sf::Keyboard::S:
				s_pressed = false;
				break;

			case sf::Keyboard::D:
				d_pressed = false;
				break;

			case sf::Keyboard::Space:
				catapult_pressed = false;
				break;
		}
	}

	//Según las teclas pulsadas podemos realizar diferentes acciones
	void Scene::update_input()
	{
		//W/S: Control del motor de aceleración, permite al vehiculo desplazarse
		if (w_pressed) playerVehicle.set_drive_engine_vel(-3);
		if (s_pressed) playerVehicle.set_drive_engine_vel( 3);

		//A/D: Control del motor de dirección, permite ajustar el ángulo de las ruedas
		if (a_pressed) playerVehicle.set_steer_engine_vel(-3);
		if (d_pressed) playerVehicle.set_steer_engine_vel( 3);

		//Al no pulsar ni W ni S. se detiene la aceleración del vehiculo
		if (!w_pressed && !s_pressed)
			playerVehicle.set_drive_engine_vel(0);

		//Si no se pulsa ni A ni D, se detiene el ajuste del ángulo de las ruedas
		if (!a_pressed && !d_pressed)
			playerVehicle.set_steer_engine_vel(0);

		//Al pulsar ESPACIO se activa la catapulta
		if(catapult_pressed)
			playerVehicle.catapult_Hinge->setMotorTargetVelocity(-10);
		
		//Al soltar ESPACIO una pequeña fuerza mueve la plataforma a su origen
		else if(!catapult_pressed)
			playerVehicle.catapult_Hinge->setMotorTargetVelocity(1);
	}
}