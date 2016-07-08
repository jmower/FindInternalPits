//
//  Patch.hpp
//  PlaneProjection
//
//  Created by Jim Mower on 5/19/16.
//  Copyright © 2016 Jim Mower. All rights reserved.
//
#ifndef Patch_hpp
#define Patch_hpp

#include <stdio.h>
//#define GLM_SWIZZLE
#include <glm/glm.hpp>

//#endif /* Patch_hpp */
class Patch {
private:
    glm::vec3 B030;
    glm::vec3 B021;
    glm::vec3 B012;
    glm::vec3 B003;
    glm::vec3 B102;
    glm::vec3 B201;
    glm::vec3 B300;
    glm::vec3 B210;
    glm::vec3 B120;
    glm::vec3 B111;
    glm::vec3 patchNormal;
    glm::vec3 vertexNormal[3]; // PN vectors for this patch
    glm::vec2 getPatchIndexFromColRow();
//    void calcPositions(glm::vec3 pnVerts[]);
    void calcPositions(void);
    void buildPatchNormal();
public:
    Patch();
    ~Patch();
    Patch (glm::vec3 _B030, glm::vec3 _B003, glm::vec3 __B300);
    const float INITVAL = -1.0f;
    glm::vec3 getB030(void);
    glm::vec3 getB021(void);
    glm::vec3 getB012(void);
    glm::vec3 getB003(void);
    glm::vec3 getB102(void);
    glm::vec3 getB201(void);
    glm::vec3 getB300(void);
    glm::vec3 getB210(void);
    glm::vec3 getB120(void);
    glm::vec3 getB111(void);
    glm::vec3 getPatchNormal(void);
    glm::vec3* getVertexNormal();
    static glm::vec3 projectToPlane(glm::vec3 point, glm::vec3 planePoint, glm::vec3 planeNormal);
};
#endif /* Patch_hpp */
