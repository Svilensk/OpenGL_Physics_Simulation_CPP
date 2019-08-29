/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef TEXTURE_2D_HEADER
#define TEXTURE_2D_HEADER

extern "C"
{
	#include <targa.h>
}

#include <vector>
#include <string>

#include "ColorBuffer_RGBA8888.hpp"

using namespace std;
namespace Anim3D_Project2
{
	class   ColorBuffer_RGBA8888;
	typedef ColorBuffer_RGBA8888 Texture;

	class Texture2D
	{
	public:

		//IDs de las texturas, tanto si se ha añadido una ruta al modelo "texture ID" como si no "Default Texture ID"
		GLuint texture_id;
		GLuint default_texture_id;

		Texture2D();


		void RenderTexture2D();		                     //Renderizado de textura 2D
		void CreateTexture(const char * texture_path);   //Creamos el buffer de la textura a partir del archivo indicado
		void SetupTexture();                             //Configuración de la textura

		//Se permite al poseer una textura2D cargar una por defecto
		void UseDefaultTexture(){ glBindTexture(GL_TEXTURE_2D, default_texture_id); };

		std::auto_ptr< Texture > load_texture(const char * texture_file_path  ); //Carga de textura a partir de un array de chars
		std::auto_ptr< Texture > load_texture(const std::string & texture_path); //Carga de textura a partir de un std::string

		std::auto_ptr< Texture > default_texture; // Ptr a la textura por defecto	
		std::auto_ptr< Texture > texture;         // Ptr a la textura cargada
	};
}

#endif