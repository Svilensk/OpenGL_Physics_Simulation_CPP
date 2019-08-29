/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef FILTER_HEADER
#define FILTER_HEADER

#include "glm/glm.hpp"
#include <assert.h>
#include <GL/glew.h>

#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Texture2D.hpp"

#include <SFML/OpenGL.hpp>

namespace Anim3D_Project2
{


	class PostProcess
	{
	public:

		// Índices para el array vbo_ids
		enum { 
			COORDINATES_VBO, TEXTURE_UVS_VBO,
			NORMALS_VBO, INDICES_IBO,
			VBO_COUNT
		};

		Shader_Program shader_program;

		Texture2D texture2d;    //Textura para el postproceso
		bool      has_texture;  //Booleano para indicar si se ha habilitado la textura

		GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs
		GLuint vao_id;                  // Id del VAO del cubo

		void render(const Camera& camera);
		PostProcess(const char*  texture_path);
	};
}

#endif // FILTER_HEADER
