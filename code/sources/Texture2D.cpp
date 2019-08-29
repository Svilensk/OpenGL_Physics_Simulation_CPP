/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "Texture2D.hpp"

namespace Anim3D_Project2
{
	Texture2D::Texture2D()
	{
		//Siempre se crea una textura por defecto
		Texture2D::default_texture = load_texture("../../assets/textures/default_texture.tga");

		//Habilitamos y vinculamos la textura por defecto en OpenGL
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &default_texture_id);
		glBindTexture(GL_TEXTURE_2D, default_texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


		//Usamos OpenGL y subimos los colores de la textura default
		glTexImage2D
		(
			GL_TEXTURE_2D, 0, GL_RGBA,
			default_texture->get_width(),
			default_texture->get_height(),
			0, GL_RGBA, GL_UNSIGNED_BYTE,
			default_texture->colors()
		);
	}

	//Dibujado de la textura
	void Texture2D::RenderTexture2D()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glDisable(GL_TEXTURE_2D);
	}

	//Carga de la textura a partir de una ruta
	void Texture2D::CreateTexture(const char * texture_path)
	{
		Texture2D::texture = load_texture(texture_path);	
	}

	void Texture2D::SetupTexture()
	{
		// Se habilitan las texturas, se genera un id para un búfer de textura,
		// se selecciona el búfer de textura creado y se configuran algunos de
		// sus parámetros

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Se suben los colores de la textura
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			texture->get_width(),
			texture->get_height(),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			texture->colors()
		);
	}


	//Sobrecarga que permite crear una textura a partir de un path string como char*
	std::auto_ptr<Texture> Texture2D::load_texture(const char * texture_file_path)
	{
		return (load_texture(std::string(texture_file_path)));
	}

	//Función de carga de imágenes para Targa.c
	std::auto_ptr<Texture> Texture2D::load_texture(const std::string & texture_path)
	{
		std::auto_ptr< Texture > texture;
		tga_image                loaded_image;

		if (tga_read(&loaded_image, texture_path.c_str()) == TGA_NOERR)
		{
			texture.reset(new Texture(loaded_image.width, loaded_image.height));

			tga_convert_depth(&loaded_image, texture->bits_per_color());
			tga_swap_red_blue(&loaded_image);

			Texture::Color * loaded_image_pixels = reinterpret_cast<Texture::Color *>(loaded_image.image_data);
			Texture::Color * loaded_image_pixels_end = loaded_image_pixels + loaded_image.width * loaded_image.height;
			Texture::Color * texture_buffer_pixels = texture->colors();

			while (loaded_image_pixels < loaded_image_pixels_end)
			{
				*texture_buffer_pixels++ = *loaded_image_pixels++;
			}

			tga_free_buffers(&loaded_image);
		}

		return (texture);
	}
}