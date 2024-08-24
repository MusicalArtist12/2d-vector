#include "World.h"
#include "Shader.h"
#include "glm/trigonometric.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

extern World world;

const double COLLISION_VARIANCE = 0.001;

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

    model = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), scale) * model;

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

// still needs to be updated with the better collision model
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

std::vector<Polygon> PhysObject::closestPolygons(PhysObject& objB) {
    if(ID == objB.ID || isGhost) { 
        return {};
    }

    double distance = glm::distance(pos, objB.pos);

    const float radiusA = radius();
    const float radiusB = objB.radius();

    std::vector<Polygon> relevantPolygons = {};

    if (distance >= radiusA + radiusB) {
        return relevantPolygons;
    }

    if (myMesh.mode != TRIANGLE) {
        std::cout << "Collision detection is only accurate for meshes using triangle indices" << std::endl;
        return relevantPolygons;
    }

    BufferMap vbo = BufferMap(&myMesh.VAO, &myMesh.VBO, GL_ARRAY_BUFFER, GL_READ_WRITE);
    BufferMap ebo = BufferMap(&myMesh.VAO, &myMesh.EBO, GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
    Vertex* vertices = (Vertex*)vbo.data;
    unsigned int* indices = (unsigned int*)ebo.data;

    for (int i = 0; i < myMesh.numIndices; i+= 3) {
        Vertex& a = vertices[indices[i]];
        Vertex& b = vertices[indices[i + 1]];
        Vertex& c = vertices[indices[i + 2]];

        glm::vec3 aPos = glm::translate(modelMatrix(), a.pos)[3];
        glm::vec3 bPos = glm::translate(modelMatrix(), b.pos)[3];
        glm::vec3 cPos = glm::translate(modelMatrix(), c.pos)[3];

        double distances[] = {
            glm::distance(aPos, objB.pos),
            glm::distance(bPos, objB.pos),
            glm::distance(cPos, objB.pos)
        };

        std::cout << objB.pos.x << ' ' << objB.pos.y << ' ' << objB.pos.z << ' ' << radiusB << '\n';
        std::cout << aPos.x << ' ' << aPos.y << ' ' << aPos.z << ' ' << distances[0] << '\n';
        std::cout << bPos.x << ' ' << bPos.y << ' ' << bPos.z << ' ' << distances[1] << '\n';
        std::cout << cPos.x << ' ' << cPos.y << ' ' << cPos.z << ' ' << distances[2] << '\n';
        

        // vertex is possibly making contact with the other object
        if (distances[0] + COLLISION_VARIANCE <= radiusB || distances[1] + COLLISION_VARIANCE<= radiusB || distances[2] + COLLISION_VARIANCE <= radiusB) {
            relevantPolygons.push_back({a, b, c});
            continue;
        }

        if (distances[0] - COLLISION_VARIANCE <= radiusB || distances[1] - COLLISION_VARIANCE<= radiusB || distances[2] - COLLISION_VARIANCE <= radiusB) {
            relevantPolygons.push_back({a, b, c});
            continue;
        }     

        glm::vec3 angles[] = {
            getTriangleAngles(aPos, bPos, objB.pos), 
            getTriangleAngles(bPos, cPos, objB.pos), 
            getTriangleAngles(cPos, aPos, objB.pos)
        };

        for (int j = 0; j < 3; j++) {
            double edgeDistanceA = glm::abs(distances[j] * glm::sin(angles[j][0]));
            double edgeDistanceB = glm::abs(distances[(j + 1) % 3] * glm::sin(angles[j][1]));

            std::cout << edgeDistanceB << ' ' << edgeDistanceB << std::endl;

            if (angles[j][0] > glm::radians(90.0) || angles[j][1] > glm::radians(90.0)) {
                continue;
            }


            if (edgeDistanceA + COLLISION_VARIANCE <= radiusB || edgeDistanceA - COLLISION_VARIANCE <= radiusB ) {
                relevantPolygons.push_back({a, b, c});
                break;
            }
        }

        std::cout << std::endl;
    }

    std::cout << "numPolygons: " << relevantPolygons.size() << std::endl;

    return relevantPolygons;
}

void PhysObject::resolveCollision(PhysObject& objB, float deltaTime) {
    if (collisionCallback == nullptr) {
        transferEnergy(objB, deltaTime);
    }
    else {
        collisionCallback(this, objB, deltaTime);
    }
}

PhysObject::PhysObject(Mesh shape, std::string id): ID(id), myMesh(shape), pos(0.0f), scale(1.0f), vel(0.0f), mass(1.0f), isStatic(false), isGhost(false), forceVectors(26), collisionCallback(nullptr) {}

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

void World::updateCollisions(PhysObject& objA, float deltaTime) {
    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& objB = objectTable.getRef(i);

        std::vector<Polygon> aPolygons = objA.closestPolygons(objB);
        std::vector<Polygon> bPolygons = objB.closestPolygons(objA);

        bool isColliding = false;

        for (int a = 0; a < aPolygons.size(); a++) {
            std::vector<glm::vec3> polyA = {
                glm::translate(objA.modelMatrix(), aPolygons[a].a.pos)[3], 
                glm::translate(objA.modelMatrix(), aPolygons[a].b.pos)[3], 
                glm::translate(objA.modelMatrix(), aPolygons[a].c.pos)[3]
            };

            for (int b = 0; b < bPolygons.size(); b++) {
                // what is the shortest possible line between each triangle?
                /*
                    cases:
                        - Edge - Edge (which in theory should just proc on edge-vertex )
                        - Edge - Vertex
                        - Vertex - Vertex 
                */
               
                std::vector<glm::vec3> polyB = {
                    glm::translate(objB.modelMatrix(), bPolygons[b].a.pos)[3], 
                    glm::translate(objB.modelMatrix(), bPolygons[b].b.pos)[3], 
                    glm::translate(objB.modelMatrix(), bPolygons[b].c.pos)[3]
                };

                // Vertex - Vertex
                for (int j = 0; j < polyA.size(); j++) {
                    for (int k = 0; k < polyB.size(); k++) {
                        double distance = glm::distance(polyA[j], polyB[k]);
                        if (distance < COLLISION_VARIANCE) {
                            isColliding = true;
                            break;
                        }
                    }

                    if (isColliding == true) {
                        break;
                    }
                }
                
                // Vertex (on A) - Edge 
                for (int j = 0; j < polyA.size(); j++) {
                    for (int k = 0; k < polyB.size(); k++) {
                        glm::vec3 angles = getTriangleAngles(polyB[k], polyB[(k + 1) % polyB.size()], polyA[j]);
                        double hyp = glm::distance(polyA[j], polyB[k]);
                        double distance = hyp * glm::sin(angles[0]);

                        if (distance <= COLLISION_VARIANCE) {
                            isColliding = true;
                            break;
                        }
                    }

                    std::cout << '\n';

                    if (isColliding == true) {
                        break;
                    }

                    
                }

                if (isColliding == true) {
                    break;
                }

                // Vertex (on B) - Edge 
                for (int j = 0; j < polyB.size(); j++) {

                    for (int k = 0; k < polyA.size(); k++) {
                        glm::vec3 angles = getTriangleAngles(polyA[k], polyA[(k + 1) % polyA.size()], polyB[j]);
                        double hyp = glm::distance(polyA[j], polyB[k]);
                        double distance = hyp * glm::sin(angles[0]);

                        if (distance <= COLLISION_VARIANCE) {
                            isColliding = true;
                            break;
                        }
                    }

                    std::cout << '\n';

                    if (isColliding == true) {
                        break;
                    }         
                }

                if (isColliding == true) {
                    break;
                }
            }
        }


        if (isColliding) {
            // std::cout << "objA: " << objA.ID << " objB: " << objB.ID << std::endl;

            if (!objA.forceVectors.hasEntry(objB.ID) || !objB.forceVectors.hasEntry(objA.ID)){
                // std::cout << "adding forces" << std::endl;

                objA.resolveCollision(objB, deltaTime);
                objB.resolveCollision(objA, deltaTime);
            }

        } 
        else {

            if (objA.forceVectors.hasEntry(objB.ID)) {
                objA.forceVectors.remove(objB.ID);
                // std::cout << "removing A forces" << std::endl;

            }

            if (objB.forceVectors.hasEntry(objA.ID)) {
                // std::cout << "removing B forces" << std::endl;

                objB.forceVectors.remove(objA.ID);
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
