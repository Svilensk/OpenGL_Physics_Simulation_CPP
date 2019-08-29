/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <cassert>
#include <fstream>
#include "Shader.hpp"

namespace Anim3D_Project2
{
	Shader::Shader(const Source_Code & source_code, GLenum shader_type): shader_object_id(0)
	{
		if (!source_code.is_empty())
		{
			//Creamos el ShaderObject
			shader_object_id = glCreateShader(shader_type);

			//Cargamos el código en OpenGL
			const char * shader_code_list[] = { source_code };
			const GLint  shader_size_list[] = { (GLint)source_code.size() };
			glShaderSource(shader_object_id, 1, shader_code_list, shader_size_list);

			//Indicamos el compilado del shader a OpenGL
			glCompileShader(shader_object_id);
		}
	}

	Shader::~Shader()
	{
		//Borramos el shader y reiniciamos su ID a 0
		glDeleteShader(shader_object_id);
		shader_object_id = 0;
	}
}