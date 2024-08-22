#include "World.h"
#include "Shader.h"

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

bool PhysObject::isColliding(PhysObject& objB) {
    if(ID == objB.ID || isGhost) { 
        return false;
    }

    double distance = glm::distance(pos, objB.pos);
    glm::vec3 direction = glm::normalize(objB.pos - pos);
    float direction_angle = glm::atan(direction[1]/direction[0]);

    if (distance >= radius() + objB.radius()) {
        return false;
    }

    if (myMesh.mode != TRIANGLE) {
        std::cout << "Collision detection is only accurate for meshes using triangle indices" << std::endl;
        return true;
    }

    // get all verticies within a certain slice of the object "radius". the closer the other object is, the wider the radius needs to be 
    // angle can be based on the other object's radius (as the length of a triangle)
    // so we have distance and height. and we know its an isolese triangle.
    // which is just two right angle triangles
    // which means tan(myAngle) = (other_radius/2) / distance
    // which means myAngle = arctan(other_radius / (distance * 2))


    // we only care about polygons that are the distance's direction += angle
    float angle = glm::atan(objB.radius() / (distance * 2));

    // to get the relevant triangles, we need to iterate over myMesh's indices (grouped by 3)
    // we'll say it matters if at least one point is inside the angle.

    struct Polygon {
        Vertex& a;
        Vertex& b;
        Vertex& c;
    };

    std::vector<Polygon> relevantPolygons;

    BufferMap vbo = BufferMap(&myMesh.VBO, GL_ARRAY_BUFFER, GL_READ_WRITE);
    Vertex* vertices = (Vertex*)vbo.data;

    for (int i = 0; i < myMesh.numIndices; i+= 3) {
        
        Vertex& a = vertices[i];
        Vertex& b = vertices[i + 1];
        Vertex& c = vertices[i + 2];

        // if any of these vertices are within direction rotated += angle, then the entire polygon matters.

        float angleA = glm::atan(a.pos[1], a.pos[0]);

        float angleB = glm::atan(b.pos[1], b.pos[0]);
        float angleC = glm::atan(c.pos[1], c.pos[0]);

        if ((direction_angle - angle <= angleA && direction_angle + angle >= angleA) ||
            (direction_angle - angle <= angleB && direction_angle + angle >= angleB) ||
            (direction_angle - angle <= angleC && direction_angle + angle >= angleC)) {
            relevantPolygons.push_back({a, b, c});
        } 

    }

    // std::cout << "angle: " << angle << " polygon ID: " << ID << "\n";
    
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

        if (obj.isColliding(iObj) && iObj.isColliding(obj)) {
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
