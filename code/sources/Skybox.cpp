/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <cassert>

#include <GL/glew.h>
#include "Skybox.hpp"
#include "Mesh.hpp"
#include <SFML/OpenGL.hpp>

#include "VertexShader.hpp"
#include "FragmentShader.hpp"

namespace Anim3D_Project2
{
	//Coordenadas de vértices del cubo que forma el SkyBox
	GLfloat sk_vertex_coordinates[] =
	{
		-1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		-1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,
		-1.0f, +1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		-1.0f, +1.0f, +1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
	};

	//vertex shader del skybox
	const std::string sk_vertex =

		"#version 330\n"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		""
		"out vec3 texture_coordinates;"
		""
		"void main()"
		"{"
		"   texture_coordinates = vec3(vertex_coordinates.x, -vertex_coordinates.y, vertex_coordinates.z);"
		"   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
		"}";

	//Fragment shader del skubox
	const std::string sk_fragment =

		"#version 330\n"
		""
		"in  vec3 texture_coordinates;"
		"out vec4 fragment_color;"
		""
		"uniform samplerCube texture_cube;"
		""
		"void main()"
		"{"
		"    fragment_color = texture (texture_cube, texture_coordinates) + vec4 ( 0.0, 0.0, 0.0, 1.0);"
		"}";


	Skybox::Skybox(const std::string & texture_base_path) : texture_cube(texture_base_path)
	{
		//Compilamos y vinculamos los shaders
		shader_program.attach(Vertex_Shader(Shader::Source_Code::from_string  (sk_vertex)));
		shader_program.attach(Fragment_Shader(Shader::Source_Code::from_string(sk_fragment)));
		shader_program.link();

		//Obtenemos las id de las matrices
		model_view_matrix_id = shader_program.get_uniform_id("model_view_matrix");
		projection_matrix_id = shader_program.get_uniform_id("projection_matrix");

		// Se generan índices para los VBOs y vinculamos el VAO
		glGenBuffers(1, &vbo_id);
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		// Se suben a un VBO los datos de coordenadas
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sk_vertex_coordinates), sk_vertex_coordinates, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}

	Skybox::~Skybox()
	{
		// Se libera el VBO y el VAO
		glDeleteVertexArrays(1, &vao_id);
		glDeleteBuffers(1, &vbo_id);
	}

	void Skybox::render(const Camera & camera)
	{
		shader_program.use();

		//bindeamos la textura del skybox
		texture_cube.bind();

		//Obtenemos las matrizes de vista y proyeccion de la cámara
		glm::mat4 model_view_matrix = camera.get_model_view();
		glm::mat4 projection_matrix = camera.get_projection();

		//Actualizamos la model view según la posición de la cámara
		model_view_matrix = glm::translate(camera.get_model_view(), glm::vec3(camera.get_location().x, camera.get_location().y, camera.get_location().z));

		//Enviamos los datos a OpenGL
		glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
		glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		//Deshabilitamos la mascara de profundidad y dibujamos el skybox
		glDepthMask(GL_FALSE);
		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		shader_program.disable();
	}
}