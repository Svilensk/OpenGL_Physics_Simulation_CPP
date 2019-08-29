/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <vector>
#include <GL/glew.h>

#include "Camera.hpp"
#include "Skybox.hpp"
#include "Model.hpp"
#include "PostProcess.hpp"

#include "BulletWorldController.hpp"
#include "Vehicle.hpp"

#include <SFML/Window.hpp>
#include <string>

namespace Anim3D_Project2
{
	class Scene
	{
	public:

		//Publico para poder hacer un reset de dynamicWorld desde el main al cerrar la ventana
	    BulletWorldController bulletWorldController;

	private:

		Camera camera; //C�mara �nica de la escena
		Skybox skybox; //Skybox �nico de la escena 
		PostProcess postprocess; //Postproceso �nico en la escena
		Vehicle playerVehicle;   //Vehiculo del jugador

		glm::vec3 cameraPos;	    //Posici�n de la c�mara en la escena
		float camera_movementSpeed; //Variable multiplicadora de la velocidad de desplazamiento de la c�mara

		float accumulated_rotation1; //Float que posee la rotaci�n acumulada de modelos en la escena

		int scene_width;  //Ancho de la ventana
		int scene_height; //Alto de la ventana

		bool w_pressed = false; //Tecla W est� siendo pulsada
		bool a_pressed = false; //Tecla A est� siendo pulsada
		bool s_pressed = false; //Tecla S est� siendo pulsada
		bool d_pressed = false; //Tecla D est� siendo pulsada

		bool catapult_pressed = false; //Tecla ESPACIO est� siendo pulsada

		bool door_destroyed = false; //Booleano que indica si la puerta ha sido abierta con la llave

	public:

		std::vector<Model*> drawableModels;  //Vector con modelos a dibujar en la escena

		Scene(int width, int height);

		void update();
		void update_input();
		void render();

		void resize(int width, int height);

		void on_key_pressed    (int key_code);
		void on_key_release    (int key_code);
	};
}

#endif