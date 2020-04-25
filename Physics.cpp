#include "Physics.hpp"

glm::mat4 btScalar2glmMat4(btScalar* matrix) {
        return glm::mat4(
            matrix[0], matrix[1], matrix[2], matrix[3],
            matrix[4], matrix[5], matrix[6], matrix[7],
            matrix[8], matrix[9], matrix[10], matrix[11],
            matrix[12], matrix[13], matrix[14], matrix[15]
        );
    }
namespace Physics {

    PhysicsWorld::PhysicsWorld(btVector3 gravity)
    {
        ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

        dynamicsWorld->setGravity(gravity);

    }

    PhysicsWorld::~PhysicsWorld() {

    }

    PhysicsBody::PhysicsBody(PhysicsWorld* w, btRigidBody* bo) {
        PhysicsBody::world = w;
        PhysicsBody::body = bo;
    }

    PhysicsBody::~PhysicsBody()
    {
    }

    btBoxShape* PhysicsWorld::createBoxShape(const btVector3& halfExtents){
        return new btBoxShape(halfExtents);
    }

    btSphereShape* PhysicsWorld::createSphereShape(const btScalar radius){
        return new btSphereShape(radius);
    }

    PhysicsBody* PhysicsWorld::createPhysicsBody(const btVector3& startPosition, btCollisionShape* shape, btScalar mass) {
        btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        //world->collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            shape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(startPosition);

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);

        return new PhysicsBody(this, body);
    }

    glm::vec3 PhysicsBody::getWorldPosition(){
        // btTransform transform = body->getWorldTransform();
        btTransform transform;
        body->getMotionState()->getWorldTransform(transform);
        
        return glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
    }

}