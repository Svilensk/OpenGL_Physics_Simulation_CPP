/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "ShaderProgram.hpp"

namespace Anim3D_Project2
{
	//Encapsulación del vinculado del shader program
	bool Shader_Program::link()
	{
		//Linkamos el shader program
		glLinkProgram(program_object_id);

		// Se comprueba si el linkage ha tenido éxito
		GLint succeeded = GL_FALSE;
		glGetProgramiv(program_object_id, GL_LINK_STATUS, &succeeded);

		//Devolvemos la validez del shader program linkado
		return (link_completed = (succeeded != GL_FALSE));
	}

	 //Encapsulación del uso del shader program
	 void Shader_Program::use() const
	 {
		assert(is_usable());
		glUseProgram(program_object_id);
	 }

	 //Encapsulación para obtener la id de un vertex attribute
	GLint Shader_Program::get_vertex_attribute_id(const char * identifier)
	{
		assert(is_usable());

		GLint  attribute_id = glGetAttribLocation(program_object_id, identifier);
		assert(attribute_id != -1);

		return (attribute_id);
	}

	//Encapsulación para obtener la id de un uniform
	GLint Shader_Program::get_uniform_id(const char * identifier)
	{
		assert(is_usable());

		GLint  uniform_id = glGetUniformLocation(program_object_id, identifier);
		assert(uniform_id != -1);

		return (uniform_id);
	}

	//Creamos el shader program y comprobamos la validez del mismo según su id
	Shader_Program::Shader_Program()
	{
		program_object_id = glCreateProgram();
		link_completed = false;
		assert(program_object_id != 0);
	}
	//El destructor libera el shader program y devuelve la id a 0
	Shader_Program::~Shader_Program()
	{
		glDeleteProgram(program_object_id);
		program_object_id = 0;
	}
}