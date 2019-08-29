/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef BULLET_WORLD_CONTROLLER_HEADER
#define BULLET_WORLD_CONTROLLER_HEADER

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

using namespace std;
namespace Anim3D_Project2
{
	class BulletWorldController
	{

	public:

		shared_ptr< btDiscreteDynamicsWorld > dynamicsWorld; //Mundo físico para bullet
		
		//Vectores a punteros de los elementos de Bullet
		vector< shared_ptr< btRigidBody          > > rigidBodies;
		vector< shared_ptr< btDefaultMotionState > > motionStates;
		vector< shared_ptr< btCollisionShape     > > collisionShapes;

		//Parámetros de configuración del mundo Bullet
		btDefaultCollisionConfiguration     collisionConfiguration;
		btCollisionDispatcher               collisionDispatcher;
		btDbvtBroadphase                    overlappingPairCache;
		btSequentialImpulseConstraintSolver constraintSolver;

		//Actualización de las físicas
		void physics_step(float Hz);

		//Comprobación de colisiones entre dos objetos con las IDs (UserIndex) indicadas
		bool check_collisions(int id_1, int id_2);

		BulletWorldController();
	};
}

#endif