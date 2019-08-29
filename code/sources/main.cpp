/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <assert.h>
#include <GL/glew.h>

#include "Scene.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;
using namespace Anim3D_Project2;

int main()
{
	//Constantes de la proporcion de pantalla a 1280x720(16:9)
	const int WINDOW_WIDTH  = 1280;
	const int WINDOW_HEIGHT = 720;

	// Se crea la ventana de SFML, que es donde se creará el contexto de OpenGL:
	Window window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Animación 3D - Práctica 2", Style::Default, ContextSettings(32));

	// Una vez se ha creado el contexto de OpenGL ya se puede inicializar Glew:
	GLenum glew_initialization = glewInit();
	assert(glew_initialization == GLEW_OK);

	//Habilitamos propiedades OpenGL
	glEnable   (GL_CULL_FACE );
	glEnable   (GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	window.setVerticalSyncEnabled(true);

	//Creamos una escena
	Scene scene(WINDOW_WIDTH, WINDOW_HEIGHT);

	bool running = true;

	do
	{
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				//Al cerrar la ventana se detiene la ejecución
				case Event::Closed:
				{
					running = false;
					break;
				}

				//Al reescalar la ventana se recalcula el tamaño de la vista y la matriz de proyeccion
				case Event::Resized:
				{
					Vector2u window_size = window.getSize();
					scene.resize(window_size.x, window_size.y);

					break;
				}

				//Al pulsar una tecla (WASD) Se mueve la cámara
				case Event::KeyPressed:
				{
					scene.on_key_pressed(event.key.code);
					break;
				}

				//Si se suelta esa tecla, se detiene el desplazamiento
				case Event::KeyReleased:
				{
					scene.on_key_release(event.key.code);
					break;
				}
			}
		}

		scene.update();

		scene.render();

		window.display();

	} while (running);

	//Destruimos el mundo físico antes que los cuerpos rígidos
	scene.bulletWorldController.dynamicsWorld.reset();

	return (EXIT_SUCCESS);
}