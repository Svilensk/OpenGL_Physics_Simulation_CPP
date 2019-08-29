/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "Mesh.hpp"
#include "tiny_obj_loader.h"

using namespace std;
using namespace tinyobj;

namespace Anim3D_Project2
{
	Mesh::Mesh(const string& obj_filePath)
	{
		//Ruta del archivo del modelo
		std::string inputfile = obj_filePath;

		//Valores del modelo
		attrib_t           model_attributes;
		vector<shape_t>    model_shapes;
		vector<material_t> model_materials;

		//Cargamos el modelo
		if (!tinyobj::LoadObj(&model_attributes, &model_shapes, &model_materials, &error, inputfile.c_str())) { return; }

		//Validamos el modelo
		if (model_shapes.size()              == 0)  { error = string("NO SHAPES IN MODEL"  ) + obj_filePath; return; }
		if (model_attributes.vertices.size() == 0)  { error = string("NO VERTICES IN MODEL") + obj_filePath; return; }
		if (model_attributes.normals.size()  == 0)  { error = string("NO SHAPES IN MODEL"  ) + obj_filePath; return; }
		 
		if (model_attributes.texcoords.size() == 0) { model_has_uvcoords = false; }
		else(model_has_uvcoords = true);

		////Reajustamos el tamaño de los vectors para Vertices, Normales, Triangulos y VertexColor del modelo
		indexes          .resize (model_shapes[0].mesh.indices.size()  );
		model_coordinates.resize (model_attributes.vertices .size() / 3);
		model_normals    .resize (model_attributes.vertices .size() / 3);
		model_uvCoords   .resize (model_attributes.vertices .size() / 3);

		//Recorremos las formas del modelo (Shapes)
		for (size_t shape = 0; shape < model_shapes.size(); shape++)
		{
			size_t index_offset = 0;

			//Recorremos las caras (Faces)
			for (size_t face = 0; face < model_shapes[shape].mesh.num_face_vertices.size(); face++)
			{
				size_t faceVertex = model_shapes[shape].mesh.num_face_vertices[face];

					indexes[index_offset + 0] = model_shapes[shape].mesh.indices[index_offset + 0].vertex_index,
					indexes[index_offset + 1] = model_shapes[shape].mesh.indices[index_offset + 1].vertex_index,
					indexes[index_offset + 2] = model_shapes[shape].mesh.indices[index_offset + 2].vertex_index;					

				//Recorremos vértices por cara (Vertex)
				for (size_t vertex = 0; vertex < faceVertex; vertex++)
				{
					tinyobj::index_t idx = model_shapes[shape].mesh.indices[index_offset + vertex];

					GLfloat vx = model_attributes.vertices[3 * idx.vertex_index + 0];
					GLfloat vy = model_attributes.vertices[3 * idx.vertex_index + 1];
					GLfloat vz = model_attributes.vertices[3 * idx.vertex_index + 2];

					model_coordinates[idx.vertex_index] = glm::vec3({ vx, vy, vz });

					if(model_has_uvcoords)
					{
						GLfloat uv1 = model_attributes.texcoords[2 * idx.texcoord_index + 0];
						GLfloat uv2 = model_attributes.texcoords[2 * idx.texcoord_index + 1];

						model_uvCoords[idx.vertex_index] = glm::vec2({ uv1, uv2 });
					}

					GLfloat nx = model_attributes.normals[3 * idx.normal_index + 0];
					GLfloat ny = model_attributes.normals[3 * idx.normal_index + 1];
					GLfloat nz = model_attributes.normals[3 * idx.normal_index + 2];

					model_normals[idx.vertex_index] = glm::vec3({ nx, ny, nz });
				}
				
				index_offset += faceVertex;
			}
		}
	}
}