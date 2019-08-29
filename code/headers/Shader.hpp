/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <string>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

namespace Anim3D_Project2
{
	class Shader
	{
	public:

		class Source_Code
		{

		private:

			//String que permite guardar la información del texto de un shader para después poder enviarlo a OpenGL
			std::string string;			
			
			Source_Code(){}
			Source_Code(const std::string & source_code_string) : string(source_code_string){}

		public:

			//Construcción del shader a partir de cadenas de texto
			static Source_Code from_string(const std::string & source_code_string) { return (Source_Code(source_code_string)); }

			//Tamaño en caracteres del shader y comprobación de si está vacío
			size_t size()     const { return (string.size());  }
			bool   is_empty() const { return (string.empty()); }

			//Operadores para devolver el string del shader como array de caracteres o como un std::String
			operator const std::string & () const{ return (Source_Code::string);}
			operator const char * () const{ return (Source_Code::string.c_str()); }
		};

	private:

		//ID del shader object vinculado
		GLuint      shader_object_id;

	protected:

		Shader(const Source_Code & source_code, GLenum shader_type);

		virtual ~Shader();

	public:

		//Validez del shader program
		bool is_compiled()        const{return (shader_object_id != 0);}

		//Getter del shader ID
		operator GLuint () const{return (shader_object_id);}
	};

}

#endif