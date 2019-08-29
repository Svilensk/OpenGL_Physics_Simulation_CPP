/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef FRAGMENT_SHADER_HEADER
#define FRAGMENT_SHADER_HEADER

#include "Shader.hpp"

namespace Anim3D_Project2
{
	class Fragment_Shader : public Shader
	{

	public:

		//Constructor de un shader tipo FragmentShader, debe recibir un código fuente
		Fragment_Shader(const Source_Code & source_code) : Shader(source_code, GL_FRAGMENT_SHADER){}
	};
}

#endif