/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef TEXTURE_CUBE_HEADER
#define TEXTURE_CUBE_HEADER

#include <memory>
/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <string>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "Texture2D.hpp"
#include "ColorBuffer_RGBA8888.hpp"


namespace Anim3D_Project2
{
	class Texture_Skybox
	{
	private:

		typedef ColorBuffer_RGBA8888 Buffer;

		GLuint texture_id;    // ID de la textura de SkyBox
		bool   load_ok;       // booleano de confirmación de carga de textura
		Texture2D texture2d;  // Textura2D creada para el skybox

	public:

		bool is_ok() const { return load_ok; } //Comprobación de la validez de la textura (conjunto)
		void bind()  const { glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id); } //Vinculado de la textura

		Texture_Skybox(const std::string & texture_base_path);
		~Texture_Skybox();
	};
}

#endif
