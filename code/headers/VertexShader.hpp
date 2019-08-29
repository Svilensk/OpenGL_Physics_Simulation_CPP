/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef VERTEX_SHADER_HEADER
#define VERTEX_SHADER_HEADER

#include "Shader.hpp"

namespace Anim3D_Project2
{
	class Vertex_Shader : public Shader
	{

	public:

		//Constructor de un shader tipo VertexShader, debe recibir un código fuente
		Vertex_Shader(const Source_Code & source_code) : Shader(source_code, GL_VERTEX_SHADER){}
	};
}

#endif