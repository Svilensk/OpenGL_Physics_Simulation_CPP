/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "BulletWorldController.hpp"

namespace Anim3D_Project2
{
	//Creamos y configuramos el mundo físico en el constructor
	BulletWorldController::BulletWorldController() : collisionDispatcher(&collisionConfiguration)
	{
		shared_ptr< btDiscreteDynamicsWorld > dynamicsWorld
		(
			new btDiscreteDynamicsWorld
			(
				&collisionDispatcher,
				&overlappingPairCache,
				&constraintSolver,
				&collisionConfiguration
			)
		);

		dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

		this->dynamicsWorld = dynamicsWorld;
	}

	//Actualizamos la física
	void BulletWorldController::physics_step(float Hz)
	{
		dynamicsWorld->stepSimulation(1.f / Hz);
	}

	//Comprobación de colisiones, devuelve true si están colisionando dos objetos con las IDs indicadas
	bool BulletWorldController::check_collisions(int id_1, int id_2)
	{
		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
			btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

			if ((obA->getUserIndex() == id_1 && obB->getUserIndex() == id_2) || (obA->getUserIndex() == id_2 && obB->getUserIndex() == id_1))
			{
				return true;
			}
		}
		return false;
	}
}