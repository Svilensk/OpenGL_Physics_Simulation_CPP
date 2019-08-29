/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <SFML/OpenGL.hpp>

namespace Anim3D_Project2
{
	class Mesh
	{
		public:
		Mesh(const std::string & model_filePath); //El constructor requiere de una ruta al modelo obligatoriamente

		std::vector<glm::vec2> model_uvCoords;    //vector que guarda las COORDENADAS UV del modelo
		std::vector<glm::vec3> model_coordinates; //vector que guarda las POSICIONES DE VERTICES del modelo
		std::vector<glm::vec3> model_normals;     //vector que guarda las NORMALES del modelo
		std::vector<unsigned int> indexes;        //vector que guarda los INDICES del modelo

		bool model_has_uvcoords; //Si el modelo no tiene uvs no se recorren para el vector de uvCoords

		std::string error; //Error de importación del modelo
	};
}
#endif