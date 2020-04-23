#ifndef PHYSICS_H
#define PHYSICS_h

#include <iostream>

#include "btBulletDynamicsCommon.h"

#include <glm/glm.hpp>

namespace Physics {

    glm::mat4 btScalar2glmMat4(btScalar* matrix);

    class PhysicsWorld;
    class PhysicsBody   {
        private:
            PhysicsWorld *world;

        public:
            btRigidBody* body;

            PhysicsBody(PhysicsWorld* w, btRigidBody* bo);
            ~PhysicsBody();

            glm::vec3 getWorldPosition();

    };
    class PhysicsWorld
    {
        private:

        public:
            btDiscreteDynamicsWorld* dynamicsWorld;
            btAlignedObjectArray<btCollisionShape*> collisionShapes;

            PhysicsWorld(btVector3 gravity);

            ~PhysicsWorld();

            btBoxShape* createBoxShape(const btVector3& halfExtents);

            btSphereShape* createSphereShape(const btScalar radius);

            PhysicsBody* createPhysicsBody(const btVector3& startPosition, btCollisionShape* shape, btScalar mass);

    };

}

#endif