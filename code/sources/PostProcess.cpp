/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <iostream>
#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>

#include "PostProcess.hpp"
#include "VertexShader.hpp"
#include "FragmentShader.hpp"



namespace Anim3D_Project2
{
	//Coordenadas del plano que contendrá la textura del postproceso
	static const GLfloat postprocess_plane_positions[] =
	{
		+1.0f, -1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f
	};

	//Coordenadas UV del plano de postproceso
	static const GLfloat postprocess_plane_uvs[] =
	{
		+1.0f,  0.0f,
		+1.0f, +1.0f,
		 0.0f, +1.0f,
 	     0.0f,  1.0f,
	     0.0f,  0.0f,
		 1.0f,  0.0f
	};

	//Código del vertex shader para el postproceso
	const std::string postprocess_vertex =

		"#version 330\n"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec2 vertex_texture_uv;"
		""
		"out vec2 texture_uv;"
		""
		"void main()"
		"{"
		"   gl_Position = vec4(vertex_coordinates, 1.0);"
		"   texture_uv  = vertex_texture_uv;"
		"}";

	//Código del fragment shader para el postproceso
	const std::string postprocess_fragment =

		"#version 330\n"
		""
		"uniform sampler2D sampler2d;"
		""
		"in  vec2 texture_uv;"
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"
		"    vec3 color = texture (sampler2d, texture_uv.st).rgb;"
		"    fragment_color = vec4((texture (sampler2d, texture_uv.st).rgb), 1.0) * vec4 ( 1.0, 1.0, 1.0, color.g);"
		"}";

	PostProcess::PostProcess(const char *  texture_path)
	{
		//Compilamos y vinculamos los shaders en el ShaderProgram
		shader_program.attach(Vertex_Shader  (Shader::Source_Code::from_string(postprocess_vertex  )));
		shader_program.attach(Fragment_Shader(Shader::Source_Code::from_string(postprocess_fragment)));
		shader_program.link();

		// Se generan índices para los VBOs y se activa el VAO
		glGenBuffers(VBO_COUNT, vbo_ids);
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		// Se suben a un VBO los datos de COORDENADAS y se vinculan al VAO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(postprocess_plane_positions), postprocess_plane_positions, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de UVS y se vinculan al VAO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(postprocess_plane_uvs), postprocess_plane_uvs, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		//Si el modelo posee una ruta de textura, se crea y vincula al modelo
		if (texture_path != 0)
		{
			texture2d.CreateTexture(texture_path);
			has_texture = texture2d.texture.get() != 0;

			if (has_texture)
			{
				texture2d.SetupTexture();
			}
		}
	}

	void PostProcess::render(const Camera & camera)
	{
		shader_program.use();

		glEnable(GL_BLEND);

		//Renderizamos la textura
		texture2d.RenderTexture2D();

		//Se bindea la textura y se dibujan los arrays de triángulos
		glBindVertexArray(vao_id);
		glBindTexture(GL_TEXTURE_2D, texture2d.texture_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisable(GL_BLEND);

		shader_program.disable();
	}
}