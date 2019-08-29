//#ifndef BULLET_BODY_HEADER
//#define BULLET_BODY_HEADER
//
//#include <btBulletDynamicsCommon.h>
//#include "Model.hpp"
//#include <memory>
//#include <vector>
//
//using namespace std;
//namespace E2_3DA
//{
//	enum shape_type
//	{
//		Sphere = 0,
//		Box    = 1,
//	};
//
//	class BulletBody
//	{
//	public:
//
//		shared_ptr< btRigidBody >      body;
//		shared_ptr< btCollisionShape > shape;
//
//		Model* relatedModel;
//
//		BulletBody(
//
//			shape_type shapetype,
//			Model* relatedModel,
//			btVector3 origin,
//			float     restitution,
//			float     size,
//			btScalar  mass,
//
//			BulletWorldController *worldController)
//	    {
//
//			shared_ptr< btCollisionShape > shape(new btSphereShape(btScalar(size)));
//
//			this->relatedModel = relatedModel;
//
//			// Create Dynamic Object
//			btTransform transform;
//			transform.setIdentity();
//			transform.setOrigin(origin);
//
//			btVector3 localInertia(0, 0, 0);
//			shape->calculateLocalInertia(mass, localInertia);
//
//			// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//			shared_ptr< btDefaultMotionState >       state(new btDefaultMotionState(transform));
//			btRigidBody::btRigidBodyConstructionInfo info(mass, state.get(), shape.get(), localInertia);
//
//			body.reset(new btRigidBody(info));
//			body->setRestitution(restitution);
//
//			worldController->dynamicsWorld->addRigidBody(body.get());
//			worldController->rigidBodies.push_back(body);
//
//			// Save the smart pointers for automatic cleanup.
//			worldController->rigidBodies.push_back(body);
//			worldController->motionStates.push_back(state);
//			worldController->collisionShapes.push_back(shape);
//		}
//
//
//		void set_shape(shape_type shapetype)
//		{
//
//
//
//
//		
//		}
//
//		void update_body()
//		{
//			btTransform physics_transform;
//			body->getMotionState()->getWorldTransform(physics_transform);
//
//			glm::mat4 graphics_transform;
//			physics_transform.getOpenGLMatrix(glm::value_ptr(graphics_transform));
//			relatedModel->set_transform(graphics_transform);
//			relatedModel->set_position(glm::vec3(graphics_transform[3][0], graphics_transform[3][1], graphics_transform[3][2]));
//		}
//	};
//}
//
//#endif