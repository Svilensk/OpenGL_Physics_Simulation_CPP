/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <vector>
#include "TextureSkybox.hpp"

extern "C"
{
#include <targa.h>
}

namespace Anim3D_Project2
{
	Texture_Skybox::Texture_Skybox(const std::string & texture_base_path)
	{
		//Funcion de carga de texturas específica para un skybox
		std::vector< std::shared_ptr< Buffer > > skybox_tex_faces(6);
		for (size_t texture_index = 0; texture_index < 6; texture_index++)
		{
			skybox_tex_faces[texture_index] = texture2d.load_texture(texture_base_path + char('0' + texture_index) + ".tga");

			if (!skybox_tex_faces[texture_index])
			{
				return;
			}
		}

		// Se crea un objeto de textura
		glEnable(GL_TEXTURE_CUBE_MAP);
		glGenTextures(1, &texture_id);

		//Activamos y configuramos la textura
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//Subimos los mapas de bits
		static const GLenum texture_target[] =
		{
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		};

		for (size_t texture_index = 0; texture_index < 6; texture_index++)
		{
			Buffer & texture = *skybox_tex_faces[texture_index];

			glTexImage2D
			(
				texture_target[texture_index],0,
				GL_RGBA,texture.get_width(),texture.get_height(),
				0,GL_RGBA,GL_UNSIGNED_BYTE,texture.colors()
			);
		}
	}

	//Liberamos las texturas al destruir el skybox
	Texture_Skybox::~Texture_Skybox()
	{
		glDeleteTextures(1, &texture_id);
	}
}
