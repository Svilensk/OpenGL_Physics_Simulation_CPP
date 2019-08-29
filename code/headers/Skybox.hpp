/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef SKYBOX_HEADER
#define SKYBOX_HEADER

#include <vector>
#include <memory>

#include "Camera.hpp"
#include "TextureSkybox.hpp"
#include "ShaderProgram.hpp"

namespace Anim3D_Project2
{
	class Skybox
	{

	private:

		GLuint vbo_id; //Id para el vbo ( coords )
		GLuint vao_id; //Id para vao

		GLint model_view_matrix_id; //id de view matrix
		GLint projection_matrix_id; //id de projection matrix

		Texture_Skybox   texture_cube; //Textura específica del cubo ( clase de textura con propiedades específicas para skybox)
		Shader_Program shader_program; //Shader program para el skybox

	public:

		void render(const Camera & camera); 

		Skybox(const std::string & texture_path);
		~Skybox();
	};
}

#endif
