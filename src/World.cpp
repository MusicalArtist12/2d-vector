#include "World.h"
#include "Shader.h"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

extern World world;

const double COLLISION_VARIANCE = 0.001;

glm::mat4 PhysObject::modelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0)) * glm::scale(glm::mat4(1.0f), scale) * model;

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
        
        // vertex is possibly making contact with the other object
        if (distances[0] + COLLISION_VARIANCE <= radiusB || distances[1] + COLLISION_VARIANCE<= radiusB || distances[2] + COLLISION_VARIANCE <= radiusB) {
            relevantPolygons.push_back({a.pos, b.pos, c.pos});
            continue;
        }

        if (distances[0] - COLLISION_VARIANCE <= radiusB || distances[1] - COLLISION_VARIANCE<= radiusB || distances[2] - COLLISION_VARIANCE <= radiusB) {
            relevantPolygons.push_back({a.pos, b.pos, c.pos});
            continue;
        }     

        Polygon polygons[] = {
            {aPos, bPos, objB.pos}, 
            {bPos, cPos, objB.pos}, 
            {cPos, aPos, objB.pos}
        };

        for (int j = 0; j < 3; j++) {  

            if (polygons[j].angle(0) > glm::radians(90.0) || polygons[j].angle(1) > glm::radians(90.0)) {
                continue;
            }
        
            double edgeDistanceA = polygons[j].height(2);
        
            if (edgeDistanceA + COLLISION_VARIANCE <= radiusB || edgeDistanceA - COLLISION_VARIANCE <= radiusB ) {
                relevantPolygons.push_back({a.pos, b.pos, c.pos});
                break;
            }
        }
    }

    return relevantPolygons;
}


PhysObject::PhysObject(Mesh shape, std::string id): ID(id), myMesh(shape), pos(0.0f), scale(1.0f), angle(0.0), vel(0.0f), mass(1.0f), isStatic(false), isGhost(false), forceVectors(26), preCollisionCallback(nullptr), postCollisionCallback(nullptr) {}

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
            updateMovement((deltaTime / (float)(countsPerFrame)) * ((float)timeScalePercent / 100.0));
        }
    }

    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& iObj = objectTable.getRef(i);
        renderQueue.queue.push_back(DrawData(&iObj.myMesh, iObj.modelMatrix()));
    }
}

void World::updateMovement(float deltaTime) {
    if (useCollisions) {
        updateCollisions(deltaTime);
    }
    
    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& iObject = objectTable.getRef(i);
        
        if (!iObject.isStatic) {
            if (useGravity) {
                addGravity(iObject);
            }
        
        }
    }

    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& iObject = objectTable.getRef(i);

        calculateMovement(iObject, deltaTime); 
    }
}

void World::updateCollisions(float deltaTime) {
    for (int i = 0; i < objectTable.size(); i++) {
        PhysObject& objA = objectTable.getRef(i);

        for (int j = i + 1; j < objectTable.size(); j++) {
            PhysObject& objB = objectTable.getRef(j);

            std::vector<Polygon> aPolygons = objA.closestPolygons(objB);
            std::vector<Polygon> bPolygons = objB.closestPolygons(objA);

            bool isColliding = false;
            bool isClipping = false;

            glm::vec3 collisionPoint; // world-space

            for (int a = 0; a < aPolygons.size(); a++) {
                Polygon polyA = {
                    glm::translate(objA.modelMatrix(), aPolygons[a].a)[3], 
                    glm::translate(objA.modelMatrix(), aPolygons[a].b)[3], 
                    glm::translate(objA.modelMatrix(), aPolygons[a].c)[3]
                };

                for (int b = 0; b < bPolygons.size(); b++) {
                    Polygon polyB = {
                        glm::translate(objB.modelMatrix(), bPolygons[b].a)[3], 
                        glm::translate(objB.modelMatrix(), bPolygons[b].b)[3], 
                        glm::translate(objB.modelMatrix(), bPolygons[b].c)[3]
                    };

                    
                    std::vector<glm::vec3> points = collisionPoints(polyA, polyB, isClipping);
                    
                    if (points.size() > 0) {
                        isColliding = true;
                        collisionPoint = glm::vec3(0.0f);

                        for (int i = 0; i < points.size(); i++) {
                            collisionPoint += points[i];
                        }

                        collisionPoint = collisionPoint / (float)points.size();

                        break;
                    }
                }
            }

            if (isColliding) {
                if (objA.preCollisionCallback != nullptr) {
                    objA.preCollisionCallback(&objA, objB, collisionPoint, deltaTime);    
                }
                if (objB.preCollisionCallback != nullptr) {
                    objB.preCollisionCallback(&objB, objA, collisionPoint, deltaTime);    
                }


                glm::vec3 objAForce = glm::sign(collisionPoint - objA.pos) * ((objA.momentum() / deltaTime)) * glm::normalize(collisionPoint - objA.pos);
                glm::vec3 objBForce = glm::sign(collisionPoint - objB.pos) * ((objB.momentum() / deltaTime)) * glm::normalize(collisionPoint - objB.pos);

                if (!objA.forceVectors.hasEntry(objB.ID)) {
                    objA.forceVectors.add(objB.ID, glm::vec3(0.0f)) = objBForce - objAForce;
                }

                if (!objB.forceVectors.hasEntry(objA.ID)) {
                    objB.forceVectors.add(objA.ID, glm::vec3(0.0f)) = objAForce - objBForce;
                }



                if (objA.postCollisionCallback != nullptr) {
                    objA.postCollisionCallback(&objA, objB, collisionPoint, deltaTime);    
                }
                if (objB.postCollisionCallback != nullptr) {
                    objB.postCollisionCallback(&objB, objA, collisionPoint, deltaTime);    
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
}


std::vector<glm::vec3> World::collisionPoints(Polygon polyA, Polygon polyB, bool& isClipping) {
    std::vector<glm::vec3> points;
    
    // Vertex - Vertex
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double distance = glm::distance(polyA[i], polyB[j]);
            if (distance < COLLISION_VARIANCE) {
                points.push_back(polyA[j]);
            }
        }
    }
    
    // Vertex (on A) - Edge 
    for (int i = 0; i < 3; i++) {
        // clipping detection

        if (glm::distance(polyA[i], polyB[0]) < polyB.height(0) &&
            glm::distance(polyA[i], polyB[1]) < polyB.height(1) && 
            glm::distance(polyA[i], polyB[2]) < polyB.height(2)) {
                isClipping = true;
                points.push_back(polyA[i]);
        }

        for (int j = 0; j < 3; j++) {
            Polygon poly = Polygon {
                polyA[i],
                polyB[j], 
                polyB[j + 1], 
            };
        
            if (poly.height(0) <= COLLISION_VARIANCE) {
                points.push_back(polyA[i]);
            }
        }
        
    }


    // Vertex (on B) - Edge 
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Polygon poly = Polygon {
                polyB[i],
                polyA[j], 
                polyA[j + 1], 
            };

            if (poly.height(0) <= COLLISION_VARIANCE) {
                points.push_back(polyA[i]);
            }
        }   
    }

    return points;
}

void World::calculateMovement(PhysObject& obj, float deltaTime) {
    
    glm::vec3 accel = obj.forceSum() / obj.mass;

    // possibly add a max-distance check here to prevent phasing through items.
    obj.vel += accel * deltaTime;
    obj.pos += obj.vel * deltaTime;

    // obj.resetForces();
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
