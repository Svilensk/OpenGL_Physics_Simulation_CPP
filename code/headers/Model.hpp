/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef CUBE_HEADER
#define CUBE_HEADER

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Texture2D.hpp"
#include "ShaderProgram.hpp"

#include <SFML/OpenGL.hpp>
#include <btBulletDynamicsCommon.h>
#include "BulletWorldController.hpp"

namespace Anim3D_Project2
 {
	 class Model
	 {
	 public:

		 //Cuerpo y forma f�sica del modelo
		 shared_ptr< btRigidBody >      body;
		 shared_ptr< btCollisionShape > shape;

		 //UserIndex aportada en el constructor sobre el modelo, guardado en el body f�sico
		 int userIndex;

		 //Booleano que indica si el modelo se debe renderizas / actualizar
		 bool is_visible;

	 private:

		 //Conversi�n de la matriz de Bullet a OpenGL, provee datos de f�sicas al modelo
		 glm::mat4 physics_matrix;

		 //Funci�n interna que configura los par�metros comunes de todos los cuerpos f�sicos
		 void internal_body_physics_configuration(float mass, float restitution, btVector3 linearFactor, BulletWorldController * worldController);

		 // �ndices para indexar el array vbo_ids:
		 enum
		 {
			 COORDINATES_VBO, TEXTURE_UVS_VBO,
			 NORMALS_VBO, INDICES_IBO,
			 VBO_COUNT
		 };


		 Shader_Program shader_program;

		 GLint  model_view_matrix_id;  //ID: Model View Matrix
		 GLint  projection_matrix_id;  //ID: Matriz de Projeccion
		 GLint  normal_matrix_id;      //ID: Normal Matrix
		 GLint  view_matrix_id;        //ID: Model View Matrix  
		 GLint  indexAmount;           //Tama�o del vector de indices del modelo

		 glm::mat4 projection_matrix;  //Matriz Projection
		 glm::mat4 model_view_matrix;  //Matriz Model View
		 glm::vec3 position;		   //Vector de POSICI�N

		 Texture2D texture2D;          //Textura del modelo
		 bool      has_texture;        //Booleano que indica si el modelo se ha construido con una textura

		 Model* parent = NULL;         //Puntero al padre (para las transformaciones jer�rquicas)
		 bool   has_parent;            //Booleano que indica si el modelo tiene padre

		 float transparency_amount;    //Float (0 - 1) con la transparencia del modelo
		 bool  is_transparent;         //Booleano que indica si el modelo es transparente

		 GLuint vbo_ids[VBO_COUNT]; // Ids de los VBOs que se usan
		 GLuint vao_id;             // Id del VAO

	 public:

		 //Sobrecarga de funci�n que permite construir diferentes formas f�sicas, esferas, cajas y formas complejas seg�n
		 //los par�metros indicados
		 void setup_body_physics(int userIndex, float mass, float restitution, float radius, btVector3 linearFactor, BulletWorldController *worldController);
		 void setup_body_physics(int userIndex, float mass, float restitution, btVector3 size, btVector3 linearFactor, BulletWorldController *worldController);
		 void setup_body_physics(shared_ptr< btCompoundShape > compound, int userIndex, float mass, float restitution, btVector3 linearFactor, BulletWorldController * worldController);

		 //Funci�n para destruir el elemento f�sico del modelo
		 void destroy(BulletWorldController *worldController);

		 glm::vec3 rotation;
		 glm::vec3 scale;
		 glm::mat4 transform;

		 glm::vec3 Model::get_position() { return position; } //Getter de la posicion
		 glm::vec3 Model::get_rotation() { return rotation; } //Getter para la rotacion

		 void Model::set_position     (glm::vec3 position)         { this->position = position; } //Setter para la posici�n del modelo
		 void Model::set_rotation     (glm::vec3 rotation)         { this->rotation = rotation; } //Setter para la rotaci�n del modelo
		 void Model::set_scale        (glm::vec3 scale   )         { this->scale    = scale;    } //Seter de la escala del modelo
		 void Model::set_transparency (float transparency_amount);                                //Setter para la transparencia del modelo

		 void Model::set_transform    (glm::mat4 transform) { this->transform = transform; } //Setter para la posici�n del modelo

		 void light_setup   (GLuint program_id); //Configuraci�n de la iluminaci�n
		 void update();						     //Update por modelo
		 void SetParent(Model* parent);          //Funci�n para establecer el padre ( para las posteriores transformaciones )

		 void render(const Camera & camera);

		 Model::Model(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, std::string obj_filePath, const char *  texture_path = 0);
		 ~Model();
	 };
 }

#endif