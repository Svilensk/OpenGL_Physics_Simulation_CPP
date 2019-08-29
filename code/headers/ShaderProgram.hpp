/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef SHADER_PROGRAM_HEADER
#define SHADER_PROGRAM_HEADER

#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include <SFML/OpenGL.hpp>

namespace Anim3D_Project2
{

	class Shader_Program
	{

	private:

		GLuint      program_object_id; //Identificador del ShaderProgram
		bool        link_completed;    //Booleano que indica si el linkado de shaders ha sido exitoso

	public:

		//Deshabilitar el shader program envía a OpenGL una instrucción de descargar todo shader program
		static void disable(){ glUseProgram(0);}

		//Comprobación de la validez del linkado
		bool is_usable() const { return (link_completed); }

		//Encapsulación de la unión de shaders para OpenGL
		void attach (const Shader & shader){ glAttachShader(program_object_id, shader);}
		void detach (const Shader & shader){ glDetachShader(program_object_id, shader); }

		//Funciones para linkado y habilitación de shaders
		bool link();
		void use() const;

		//Encapsulación de setters de openGL para diferentes tipos de datos
		void set_uniform_value(GLint uniform_id, const GLint     & value ) { glUniform1i(uniform_id, value); }
		void set_uniform_value(GLint uniform_id, const float     & value ) { glUniform1f(uniform_id, value); }
		void set_uniform_value(GLint uniform_id, const glm::vec2 & vector) { glUniform2f(uniform_id, vector[0], vector[1]); }
		void set_uniform_value(GLint uniform_id, const glm::vec3 & vector) { glUniform3f(uniform_id, vector[0], vector[1], vector[2]); }
		void set_uniform_value(GLint uniform_id, const glm::vec4 & vector) { glUniform4f(uniform_id, vector[0], vector[1], vector[2], vector[3]); }
		void set_uniform_value(GLint uniform_id, const glm::mat2 & matrix) { glUniformMatrix2fv(uniform_id, 1, GL_FALSE, glm::value_ptr(matrix)); }
		void set_uniform_value(GLint uniform_id, const glm::mat3 & matrix) { glUniformMatrix3fv(uniform_id, 1, GL_FALSE, glm::value_ptr(matrix)); }
		void set_uniform_value(GLint uniform_id, const glm::mat4 & matrix) { glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(matrix)); }

		//Encapsulación de atributos de vértices OpenGL
		void set_vertex_attribute(GLint attribute_id, const float    & value)   { glVertexAttrib1f(attribute_id, value); }
		void set_vertex_attribute(GLint attribute_id, const glm::vec2 & vector) { glVertexAttrib2fv(attribute_id, &vector[0]); }
		void set_vertex_attribute(GLint attribute_id, const glm::vec3 & vector) { glVertexAttrib3fv(attribute_id, &vector[0]); }
		void set_vertex_attribute(GLint attribute_id, const glm::vec4 & vector) { glVertexAttrib4fv(attribute_id, &vector[0]); }

		//Getters de uniform values y de vertex attributes
		GLint get_vertex_attribute_id(const char * identifier);
		GLint get_uniform_id(const char * identifier);

		//Getter de la ID del ShaderProgram
		operator GLuint () const { return (program_object_id); }

		Shader_Program ();
		~Shader_Program();
	};
}

#endif