#include "World.h"
#include "Shader.h"
#include "glm/trigonometric.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

extern World world;

float getVecAngle(glm::vec3 vector) {
    if (glm::length(vector) == 0.0) {
        return 0.0f;
    }
    
    float angle1;

    if(vector.x > 0) angle1 = glm::atan(vector.y/vector.x);
    else if(vector.x < 0) angle1 = glm::atan(vector.y/vector.x) - glm::radians(180.0f);
    else angle1 = glm::radians(90.0f) * vector.y/glm::abs(vector.y);

    return angle1;
}

glm::mat4 PhysObject::modelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(glm::mat4(1.0f), pos) * model;

    return model;
}

glm::vec3 PhysObject::forceSum() {
    glm::vec3 forces = glm::vec3(0.0f);

    if (isStatic) {
        return forces;
    }

    for (int i = 0; i < forceVectors.size(); i++) {
        if (!world.isValidForce(forceVectors.getID(i))) {
            forceVectors.remove(forceVectors.getID(i)); 
        }
        else {
            forces += forceVectors.getRef(i);
        }
    }

    return forces;
}

void PhysObject::transferEnergy(PhysObject& objB, float deltaTime) {    
    glm::vec3& forceA = forceVectors.add(objB.ID, glm::vec3(0.0f));

    glm::vec3 AB = objB.pos - pos;

    float angle1 = getVecAngle(AB);
    float angleAp = getVecAngle(momentum()) - angle1;
    float angleBp = getVecAngle(objB.momentum()) - angle1;
    float angleAf = getVecAngle(forceSum()) - angle1;

    glm::vec3 f_effA = glm::normalize(AB) * glm::length((forceSum())) * glm::cos(angleAf);
    glm::vec3 p_effB = glm::normalize(AB) * glm::length(objB.momentum()) * glm::cos(angleBp);
    glm::vec3 p_effA = glm::normalize(AB) * glm::length(momentum()) * glm::cos(angleAp);

    forceA = (p_effB - p_effA)/deltaTime - f_effA;

}

// assumes all points have the same origin
glm::vec3 getTriangleAngles(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    // angle a
    glm::vec3 p = b - a;
    glm::vec3 q = c - a;

    double angleA = glm::abs(glm::atan(p.y, p.x) - glm::atan(q.y, q.x));

    if (angleA > glm::radians(180.0)) {
        angleA = glm::radians(360.0) - angleA; 
    }

    // angle b 
    p = a - b;
    q = c - b;

    double angleB = glm::abs(glm::atan(p.y, p.x) - glm::atan(q.y, q.x));

    if (angleB > glm::radians(180.0)) {
        angleB = glm::radians(360.0) - angleB;
    }

    // angle c 
    p = a - c;
    q = b - c;

    double angleC = glm::abs(glm::atan(p.y, p.x) - glm::atan(q.y, q.x));

    if (angleC > glm::radians(180.0)) {
        angleC = glm::radians(360.0) - angleC;
    }

    return glm::vec3(angleA, angleB, angleC);
}

bool PhysObject::isColliding(PhysObject& objB) {
    if(ID == objB.ID || isGhost) { 
        return false;
    }

    double distance = glm::distance(pos, objB.pos);

    const float radiusA = radius();
    const float radiusB = objB.radius();

    if (distance >= radiusA + radiusB) {
        return false;
    }

    if (myMesh.mode != TRIANGLE) {
        std::cout << "Collision detection is only accurate for meshes using triangle indices" << std::endl;
        return true;
    }

    struct Polygon {
        Vertex& a;
        Vertex& b;
        Vertex& c;
    };

    std::vector<Polygon> relevantPolygons;

    BufferMap vbo = BufferMap(&myMesh.VAO, &myMesh.VBO, GL_ARRAY_BUFFER, GL_READ_WRITE);
    BufferMap ebo = BufferMap(&myMesh.VAO, &myMesh.EBO, GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
    Vertex* vertices = (Vertex*)vbo.data;
    unsigned int* indices = (unsigned int*)ebo.data;

    for (int i = 0; i < myMesh.numIndices; i+= 3) {
        Vertex& a = vertices[indices[i]];
        Vertex& b = vertices[indices[i + 1]];
        Vertex& c = vertices[indices[i + 2]];

        // relative to object A
        glm::vec3 aPos = glm::make_vec3(a.pos);
        glm::vec3 bPos = glm::make_vec3(b.pos);
        glm::vec3 cPos = glm::make_vec3(c.pos);

        double distanceA = 0.95 * glm::distance(aPos, objB.pos - pos);
        double distanceB = 0.95 * glm::distance(bPos, objB.pos - pos);   
        double distanceC = 0.95 * glm::distance(cPos, objB.pos - pos);   

        
        // vertex is possibly making contact with the other object
        if (distanceA <= radiusB || distanceB <= radiusB || distanceC <= radiusB) {
            relevantPolygons.push_back({a, b, c});
            continue;
        }
        

        glm::vec3 anglesA = getTriangleAngles(aPos, bPos, objB.pos - pos);
        glm::vec3 anglesB = getTriangleAngles(bPos, cPos, objB.pos - pos);
        glm::vec3 anglesC = getTriangleAngles(cPos, aPos, objB.pos - pos);

        std::cout << "anglesA " << glm::degrees(anglesA[0]) << ' ' << glm::degrees(anglesA[1]) << ' ' << glm::degrees(anglesA[2]) << '\n';
        std::cout << "anglesB " << glm::degrees(anglesB[0]) << ' ' << glm::degrees(anglesB[1]) << ' ' << glm::degrees(anglesB[2]) << '\n';
        std::cout << "anglesC " << glm::degrees(anglesC[0]) << ' ' << glm::degrees(anglesC[1]) << ' ' << glm::degrees(anglesC[2]) << '\n';

        if (anglesA[0] <= glm::radians(90.0) && anglesA[1] <= glm::radians(90.0) && anglesA[2] <= glm::radians(90.0)) {
            double edgeDistance = glm::abs(0.95 * distanceA * glm::sin(anglesA[0]));

            if (edgeDistance <= radiusB) {
                relevantPolygons.push_back({a, b, c});
                continue;
            }
        }
        
        if (anglesB[0] <= glm::radians(90.0) && anglesB[1] <= glm::radians(90.0) && anglesB[2] <= glm::radians(90.0)) {
            double edgeDistance = glm::abs(0.95 * distanceB * glm::sin(anglesB[0]));

            if (edgeDistance <= radiusB) {
                relevantPolygons.push_back({a, b, c});
                continue;
            }
        }
        if (anglesC[0] <= glm::radians(90.0) && anglesC[1] <= glm::radians(90.0) && anglesC[2] <= glm::radians(90.0)) {
            double edgeDistance = glm::abs(0.95 * distanceC * glm::sin(anglesC[0]));
 
            if (edgeDistance <= radiusB) {
                relevantPolygons.push_back({a, b, c});
                continue;
            }

        }
    }

    std::cout << "number of relevant polygons: " << relevantPolygons.size() << '\n';
    
    if (ID == "left wall" && relevantPolygons.size() > 0) {
        std::cout << "\n\n\n";

        world.isPaused = true;
    }


    if (relevantPolygons.size() == 0) {

        return false;
    }

    for (int i = 0; i < relevantPolygons.size(); i++) {
        // std::cout << relevantPolygons[i].a.pos[0] << ' ' << relevantPolygons[i].a.pos[1] << ' ' <<  relevantPolygons[i].a.pos[2] << '\n';
        // std::cout << relevantPolygons[i].b.pos[0] << ' ' << relevantPolygons[i].b.pos[1] << ' ' <<  relevantPolygons[i].b.pos[2] << '\n';
        // std::cout << relevantPolygons[i].c.pos[0] << ' ' << relevantPolygons[i].c.pos[1] << ' ' <<  relevantPolygons[i].c.pos[2] << "\n\n";

        relevantPolygons[i].a.rgba[0] = 1.0;
        relevantPolygons[i].a.rgba[1] = 0.0;
        relevantPolygons[i].a.rgba[2] = 0.0;

        relevantPolygons[i].b.rgba[0] = 1.0;
        relevantPolygons[i].b.rgba[1] = 0.0;
        relevantPolygons[i].b.rgba[2] = 0.0;

        relevantPolygons[i].c.rgba[0] = 1.0;
        relevantPolygons[i].c.rgba[1] = 0.0;
        relevantPolygons[i].c.rgba[2] = 0.0;

    }


    return true;
}

void PhysObject::resolveCollision(PhysObject& objB, float deltaTime) {
    if (collisionCallback == nullptr) {
        transferEnergy(objB, deltaTime);
    }
    else {
        collisionCallback(this, objB, deltaTime);
    }
}

PhysObject::PhysObject(Mesh shape, std::string id): ID(id), myMesh(shape), pos(0.0f), vel(0.0f), mass(1.0f), isStatic(false), isGhost(false), forceVectors(26), collisionCallback(nullptr) {}

void PhysObject::addKeyCallback(std::string name, int key, int state, void (*callback)(PhysObject&, KeyCallback* self)) {
    callbacks.push_back({
        key,
        state,
        callback,
        name
    });
}

void PhysObject::removeKeyCallback(std::string name) {
    for (int i = 0; i < callbacks.size(); i++) {
        if (callbacks[i].name == name) {
            callbacks.erase(callbacks.begin()+i);
        }
    }
}

void PhysObject::inputLoop(Window& window) {
    for (int i = 0; i < callbacks.size(); i++) {
        if (window.readKey(callbacks[i].key) == callbacks[i].state) {
            callbacks[i].callback(*this, &callbacks[i]);
        }
    }
}

// ******************************************************************

void World::update(Window& window, RenderQueue& renderQueue) {
    float deltaTime = window.deltaTime > 0 ? window.deltaTime : 0;
   
    if (!isPaused) {
        for (int i = 0; i < objectTable.size(); i++) {
            PhysObject& iObj = objectTable.getRef(i);
            iObj.inputLoop(window);
        }

        for (int i = 0; i < countsPerFrame; i++) {
            updateMovement(deltaTime/(float)(countsPerFrame));
        }
    }

    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& iObj = objectTable.getRef(i);
        renderQueue.queue.push_back(DrawData(&iObj.myMesh, iObj.modelMatrix()));
    }
}

void World::updateMovement(float deltaTime) {
    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& iObject = objectTable.getRef(i);
        
        if (!iObject.isStatic) {
            if (useGravity) {
                addGravity(iObject);
            }
            
            if (useCollisions) {
                updateCollisions(iObject, deltaTime);
            }
        }
    }

    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& iObject = objectTable.getRef(i);

        calculateMovement(iObject, deltaTime); 
    }
}

void World::updateCollisions(PhysObject& obj, float deltaTime) {
    for (int j = 0; j < objectTable.size(); j++) {
        PhysObject& iObj = objectTable.getRef(j);

        bool testA = obj.isColliding(iObj);
        bool testB = iObj.isColliding(obj);

        if (testA && testB) {
            if (!obj.forceVectors.hasEntry(iObj.ID) || !iObj.forceVectors.hasEntry(obj.ID)){
                obj.resolveCollision(iObj, deltaTime);
                iObj.resolveCollision(obj, deltaTime);
            }
        } 
        else {
            if (obj.forceVectors.hasEntry(iObj.ID)) {
                obj.forceVectors.remove(iObj.ID);
            }

            if (iObj.forceVectors.hasEntry(obj.ID)) {
                iObj.forceVectors.remove(obj.ID);
            }
        }
    }  
}

void World::calculateMovement(PhysObject& obj, float deltaTime) {
    
    glm::vec3 accel = obj.forceSum() / obj.mass;

    // possibly add a max-distance check here to prevent phasing through items.

    obj.vel += accel * deltaTime;
    obj.pos += obj.vel * deltaTime;
}

void World::addGravity(PhysObject& obj) {
    glm::vec3& gravForce = obj.forceVectors.add("gravForce", glm::vec3(0.0f));
    gravForce = glm::vec3(0.0f, grav, 0.0f) * obj.mass;
}

bool World::isValidForce(std::string ID) {
    if (ID == "") {
        return false;
    }

    if (ID == "gravForce" || ID == "External Force") { 
        return true;
    }

    for (int i = 0; i < objectTable.size(); i++) {
        if (objectTable.getID(i) == ID) {
            return true;
        }
    }

    return false;
}
