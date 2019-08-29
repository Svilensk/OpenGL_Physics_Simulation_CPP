/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <GL/glew.h> 
#include "Model.hpp"
#include <SFML/OpenGL.hpp>

#include "VertexShader.hpp"
#include "FragmentShader.hpp"

#include <algorithm>

namespace Anim3D_Project2
{
	//Código del vertex shader para los modelos de la escena
	const std::string model_vertex =

		"#version 330\n"
		""
		"struct Light"
		"{"
		"    vec4 position;"
		"    vec3 color;   "
		"};"
		""
		"uniform Light light;"
		"uniform float ambient_intensity;"
		"uniform float diffuse_intensity;"
		"uniform float alpha;"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		"uniform mat4     normal_matrix;"
		"uniform mat4       view_matrix;"
		""
		"uniform vec3 material_color;"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec3 vertex_normal;     "
		"layout (location = 2) in vec2 vertex_texture_uv; "
		""
		"out vec3 front_color;"
		"out vec2 texture_uv; "
		"out float out_alpha; "
		""
		"void main()"
		"{"
		"    vec4  normal   = normal_matrix     * vec4(vertex_normal, 1.0);     "
		"    vec4  position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
		"    vec4  world_space_light_pos = (view_matrix * light.position);      "
		""  
		"    vec4  light_direction = normalize(world_space_light_pos - position);"
		"    float light_intensity = diffuse_intensity * max (dot (normalize (normal.xyz), normalize(light_direction.xyz)), 0.0);"
		""
		"    out_alpha = alpha;              "
		"    texture_uv  = vertex_texture_uv;"
		"    front_color = ambient_intensity + diffuse_intensity * light_intensity * light.color;"
		"    gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
		"}";

	//Código del fragment shader para los modelos de la escena
	const std::string model_fragment =

		"#version 330\n"
		""
		"uniform sampler2D sampler2d;"
		""
		"in  vec3 front_color;   "
		"in  vec2 texture_uv;    "
		"in  float out_alpha;    "
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"
		"    fragment_color = vec4((texture (sampler2d, texture_uv.st).rgb) * front_color, 1.0) * vec4 ( 1.0, 1.0, 1.0, out_alpha);;"
		"}";

	Model::Model(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, std::string obj_filePath, const char *  texture_path)
		: position(position),
		has_texture   (false),
		has_parent    (false),
		is_transparent(false),
		is_visible    (true ),
		scale     (scale),
		rotation  (rotation)
	{
		//Creamos una mesh en el modelo
		Mesh newMesh(obj_filePath);
		indexAmount = newMesh.indexes.size();

		//Compilamos y vinculamos los shaders en el ShaderProgram
		shader_program.attach(Vertex_Shader  (Shader::Source_Code::from_string(model_vertex)));
		shader_program.attach(Fragment_Shader(Shader::Source_Code::from_string(model_fragment)));
		shader_program.link();

		//Obtenemos la ID de las matrices de vista y proyección
		model_view_matrix_id = shader_program.get_uniform_id("model_view_matrix");
		projection_matrix_id = shader_program.get_uniform_id("projection_matrix");

		// Se generan índices para los VBOs y se activa el VAO
		glGenBuffers(VBO_COUNT, vbo_ids);
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		// Se suben a un VBO los datos de COORDENADAS y se vinculan al VAO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
		glBufferData(GL_ARRAY_BUFFER, newMesh.model_coordinates.size() * sizeof(glm::vec3), &newMesh.model_coordinates[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de NORMALES y se vinculan al VAO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, newMesh.model_normals.size() * sizeof(glm::vec3), &newMesh.model_normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de UVS y se vinculan al VAO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, newMesh.model_uvCoords.size() * sizeof(glm::vec2), &newMesh.model_uvCoords[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un IBO los datos de índices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.indexes.size() * sizeof(unsigned int), &newMesh.indexes[0], GL_STATIC_DRAW);

		glBindVertexArray(0);

		//Si el modelo posee una ruta de textura, se crea y vincula al modelo
		if(texture_path != 0)
		{
			texture2D.CreateTexture(texture_path);
			has_texture = texture2D.texture.get() != 0;

			if(has_texture)
			{
				texture2D.SetupTexture();			
			}
		}

		//Configuramos la iluminación en el modelo
		shader_program.use();
		light_setup(shader_program);
		shader_program.disable();
	}			

	//Establecemos el padre del modelo si se desea, guardando su referencia para usar su matriz al aplicar la transformación
	void Model::SetParent(Model* parent)
	{
		has_parent   = true;
		this->parent = parent;
	}

	//La transparencia del modelo recibe un float (0 - 1) con el valor de la transparencia
	void Model::set_transparency(float transparency_amount)
	{
		this->transparency_amount = std::max (0.0f , std::min(transparency_amount, 1.0f));

		if (this->transparency_amount < 1.f) is_transparent = true;
		else  is_transparent = false; 
	}

	//Función de renderizado llamada cada frame
	void Model::render(const Camera & camera)
	{
		//Habilitamos el shader program
		shader_program.use();

		//Si el modelo es transparente, habilitamos el blending y modificamos el alpha según el valor indicado en "SetTransparency"
		if(is_transparent)
		{
			glEnable(GL_BLEND);
			GLint alpha_position = glGetUniformLocation(shader_program, "alpha");
			glUniform1f(alpha_position, transparency_amount);
		}

		GLint view_pos = glGetUniformLocation(shader_program, "view_matrix");

		//Obtenemos las matrices de la cámara
		model_view_matrix           = camera.get_model_view();
		glm::mat4 view_matrix       = camera.get_model_view();
		glm::mat4 projection_matrix = camera.get_projection();

	   //model_view_matrix = camera.get_model_view() * transform;

		glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(view_matrix));

		//Si tiene padre el modelo, la matriz se modifica respecto a la del padre
		if(has_parent){ model_view_matrix = parent->model_view_matrix; }

		//Aplicamos las transformaciones a la matriz
		model_view_matrix = model_view_matrix * physics_matrix;
		//model_view_matrix = glm::translate(model_view_matrix, glm::vec3(position.x, position.y, position.z));

		model_view_matrix = glm::rotate   (model_view_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model_view_matrix = glm::rotate   (model_view_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model_view_matrix = glm::rotate   (model_view_matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		model_view_matrix = glm::scale    (model_view_matrix, scale);

		//Vinculamos los uniforms modificados con OpenGL
		glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
		glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));

		//Obtenemos la matriz normal
		GLint n_matrix = glGetUniformLocation(shader_program, "normal_matrix");
		glUniformMatrix4fv(n_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

		//Aplicamos la textura del modelo
		if (has_texture) texture2D.RenderTexture2D();
		else texture2D.UseDefaultTexture();	

		// Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos
		glBindVertexArray(vao_id);
		glDrawElements(GL_TRIANGLES, indexAmount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Si el modelo era transparente, deshabilitamos el blending previamente activo
		if (is_transparent) glDisable(GL_BLEND);

		shader_program.disable();
	}

	//Funcion de actualización del modelo, permite actualizar la matriz del modelo a partir de los cálculos
	//realizados por el motor de físicas
	void Model::update()
	{
		//Obtenemos la transformación física del cuerpo físico relacionado al modelo
		btTransform physics_transform;
		body->getMotionState()->getWorldTransform(physics_transform);

		//Convertimos la matriz de transformación a una matriz OpenGL para aplicarsela al modelo
		glm::mat4 graphics_transform;
		physics_transform.getOpenGLMatrix(glm::value_ptr(physics_matrix));
		this->set_transform(physics_matrix);

		//Actualizo la posición del modelo (parámetro del modelo que posee sus coordenadas XYZ, para el facil acceso a los datos)
		this->set_position(glm::vec3(physics_matrix[3][0], physics_matrix[3][1], physics_matrix[3][2]));
	}

	//Estas tres funciones presentan una sobrecarga que permite crear diferentes formas para el cuerpo físico del modelo según los parámetros aportados
	//Una vez los parámetros individuales y característicos de cada forma se han asignado, se llama a la función internal_body_physics_configuration().
	//La cual posee la configuración genérica sin importar el tipo de forma del modelo

	//CONFIG. FORMA COMPUESTA
	void Model::setup_body_physics(shared_ptr< btCompoundShape > compound, int userIndex, float mass, float restitution, btVector3 linearFactor, BulletWorldController * worldController)
	{
		//Guardamos el índice
		this->userIndex = userIndex;

		//Creamos una forma COMPUESTA
		shared_ptr< btCollisionShape > nshape(compound);
		this->shape = nshape;

		//configuramos el resto de los parámetros
		internal_body_physics_configuration(mass, restitution, linearFactor, worldController);
	}

	//CONFIG. FORMA ESFÉRICA
	void Model::setup_body_physics(int userIndex, float mass, float restitution, float radius, btVector3 linearFactor, BulletWorldController * worldController)
	{
		//Guardamos el índice
		this->userIndex = userIndex;

		//Creamos una forma ESFERA
		shared_ptr< btCollisionShape > nshape(new btSphereShape(btScalar(radius)));
		this->shape = nshape;

		//configuramos el resto de los parámetros
		internal_body_physics_configuration(mass, restitution, linearFactor, worldController);
	}

	//CONFIG. FORMA DE CAJA
	void Model::setup_body_physics(int userIndex, float mass, float restitution, btVector3 size, btVector3 linearFactor, BulletWorldController *worldController)
	{
		//Guardamos el índice
		this->userIndex = userIndex;

		//Creamos una forma CAJA
		shared_ptr< btCollisionShape > nshape(new btBoxShape(size));
		this->shape = nshape;

		//configuramos el resto de los parámetros
		internal_body_physics_configuration(mass, restitution, linearFactor, worldController);
	}

	//CONFIG. GENÉRICA A CUALQUIER FORMA
	void Model::internal_body_physics_configuration(float mass, float restitution, btVector3 linearFactor, BulletWorldController * worldController)
	{
		//Situamos el origen del cuerpo físico en el origen del modelo 3D
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(position.x, position.y, position.z));

		//Calculamos la inercia del modelo
		btVector3 localInertia(0, 0, 0);
		shape->calculateLocalInertia(mass, localInertia);

		//Configuramos las propiedades básicas de construcción del cuerpo
		shared_ptr< btDefaultMotionState >       state(new btDefaultMotionState(transform));
		btRigidBody::btRigidBodyConstructionInfo info(mass, state.get(), shape.get(), localInertia);

		//Establecemos los parámetros que recibidos como parámetro
		body.reset(new btRigidBody(info));
		body->setRestitution(restitution);
		body->setUserIndex(this->userIndex);
		body->setLinearFactor(linearFactor);

		//Por defecto, todos los modelos están bloqueados en el espacio en X,Z así como sus ejes de rotación
		body->setAngularFactor(btVector3(0, 0, 0));
		body->setGravity      (btVector3(0, 1, 0));
		
		//Añadimos el cuerpo al mundo dinámico
		worldController->dynamicsWorld->addRigidBody(body.get());

		//Guardamos la información del modelo en vectores de punteros
		worldController->rigidBodies.push_back(body);
		worldController->motionStates.push_back(state);
		worldController->collisionShapes.push_back(shape);
	}

	//Configuración de iluminación en modelos
	void Model::light_setup(GLuint program_id)
	{
		//Coordenadas y color de la luz
		GLint light_position    = glGetUniformLocation(program_id, "light.position");
		GLint light_color       = glGetUniformLocation(program_id, "light.color");

		//Intensidad de iluminación ambiente y difusa
		GLint ambient_intensity = glGetUniformLocation(program_id, "ambient_intensity");
		GLint diffuse_intensity = glGetUniformLocation(program_id, "diffuse_intensity");

		//Uniforms que se pasan a OpenGL con los valores de posición, color e intensidad
		glUniform4f(light_position, 100.f, 50.f, 0.f, 1.f);
		glUniform3f(light_color, 1.f, 1.f, 1.f);
		glUniform1f(ambient_intensity, 0.2f);
		glUniform1f(diffuse_intensity, 1.f);
	}

	//Función que permite destruir un cuerpo físico del mundo
	void Model::destroy(BulletWorldController* worldController)
	{
		//Obtenemos todos los componentes de colision del mundo
		btCollisionObjectArray arr = worldController->dynamicsWorld->getCollisionObjectArray();

		//ID del cuerpo que deseamos destruir
		int deleteID;

		//Buscamos entre todos los componentes aquel con el ID que se desea borrar, guardamos su ID y lo eliminamos del mundo
		for (size_t i = 0; i < arr.size(); i++)
		{
			if (arr.at(i)->getUserIndex() == userIndex) deleteID = i;
		}
		worldController->dynamicsWorld->removeCollisionObject(worldController->dynamicsWorld->getCollisionObjectArray()[deleteID]);
	}

	Model::~Model()
	{
		//El destructor libera las texturas	que el modelo poseía
		if (has_texture) glDeleteTextures(1, &texture2D.texture_id);
		else glDeleteTextures(1, &texture2D.default_texture_id);

		//Además, se liberan los VBOs y el VAO usados
		glDeleteVertexArrays(1, &vao_id);
		glDeleteBuffers(VBO_COUNT, vbo_ids);
	}
}
