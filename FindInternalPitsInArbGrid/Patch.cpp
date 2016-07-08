//
//  Patch.cpp
//  PlaneProjection
//
//  Created by Jim Mower on 5/19/16.
//  Copyright © 2016 Jim Mower. All rights reserved.
//
#include "stdafx.h"

#include <iostream>
#include "Patch.hpp"


Patch::Patch() {
    
}
Patch::~Patch() {
    
}

Patch::Patch(glm::vec3 _B030, glm::vec3 _B003, glm::vec3 __B300) {
    B030 = _B030;
    B021 = glm::vec3(INITVAL);
    B012 = glm::vec3(INITVAL);
    B003 = _B003;
    B102 = glm::vec3(INITVAL);
    B201 = glm::vec3(INITVAL);
    B300 = __B300;
    B210 = glm::vec3(INITVAL);
    B120 = glm::vec3(INITVAL);
    B111 = glm::vec3(INITVAL);
    buildPatchNormal();
    calcPositions();
    std::cout << "B030: " << B030.x << ", " << B030.y << ", " << B030.z << std::endl;
    std::cout << "B003: " << B003.x << ", " << B003.y << ", " << B003.z << std::endl;
    std::cout << "B300: " << B300.x << ", " << B300.y << ", " << B300.z << std::endl;
    std::cout << "B021: " << B021.x << ", " << B021.y << ", " << B021.z << std::endl;
    std::cout << "B012: " << B012.x << ", " << B012.y << ", " << B012.z << std::endl;
    std::cout << "B102: " << B102.x << ", " << B102.y << ", " << B102.z << std::endl;
    std::cout << "B201: " << B201.x << ", " << B201.y << ", " << B201.z << std::endl;
    std::cout << "B210: " << B210.x << ", " << B210.y << ", " << B210.z << std::endl;
    std::cout << "B120: " << B120.x << ", " << B120.y << ", " << B120.z << std::endl;
    std::cout << "B111: " << B111.x << ", " << B111.y << ", " << B111.z << std::endl;
}
glm::vec3 Patch::projectToPlane(glm::vec3 point, glm::vec3 planePoint, glm::vec3 normal) {
    // This projects the incoming point to the plane described by a point on the plane (planePoint) and a normal. For
    // this project, a normal is a point normal at point (for PN triangles).
    // I use it to pop a pit (and 5 of the 8 surrounding grid points) out to the plane of 3 of its 8-surrounding points.
    glm::vec3 v = point - planePoint;
    float len = glm::dot(v, normal);
    glm::vec3 d = len * normal;
    return (point - d);  // desired
}

//void Patch::calcPositions(glm::vec3 pnVerts[]) {
void Patch::calcPositions() {

    // Simulator for GDVtcs.glsl::CalcPositions()
    
    // The original vertices stay the same. Bottom triangle is ordered UL, LL, RR. Top triangle is ordered UL, LR, UR.
//    B030 = pnVerts[0];
//    B003 = pnVerts[1];
//    B300 = pnVerts[2];
    // Multiply original vertices by Z multiplier--this works, but hold off until I get equilateral tessellation working
    //B030.y *= gZMultiplier;
    //B003.y *= gZMultiplier;
    //B300.y *= gZMultiplier;
    
    // Edges are names according to the opposing vertex
    glm::vec3 EdgeB300 = B003 - B030;
    glm::vec3 EdgeB030 = B300 - B003;
    glm::vec3 EdgeB003 = B030 - B300;
    
    // Generate two midpoints on each edge
    const float ONETHIRD = 1.0f / 3.0f;
    const float TWOTHIRDS = ONETHIRD * 2.0f;
    const float ONESIXTH = 1.0f / 6.0f;

    glm::vec3 midPoint = B030 + EdgeB300 * ONETHIRD;
    B021 = midPoint;
    glm::vec3 midPoint2 = B030 + EdgeB300 * TWOTHIRDS;
    B012 = midPoint2;
    glm::vec3 midPoint3 = B003 + EdgeB030 * ONETHIRD;
    B102 = midPoint3;
    glm::vec3 midPoint4 = B003 + EdgeB030 * TWOTHIRDS;
    B201 = midPoint4;
    glm::vec3 midPoint5 = B300 + EdgeB003 * ONETHIRD;
    B210 = midPoint5;
    glm::vec3 midPoint6 = B300 + EdgeB003 * TWOTHIRDS;
    B120 = midPoint6;
    
    // Project each midpoint on the plane defined by the nearest vertex and its normal
    B021 = projectToPlane(B021, B030, vertexNormal[0]);  // desired
    B012 = projectToPlane(B012, B003, vertexNormal[1]);
    B102 = projectToPlane(B102, B003, vertexNormal[1]);
    B201 = projectToPlane(B201, B300, vertexNormal[2]);
    B210 = projectToPlane(B210, B300, vertexNormal[2]);
    B120 = projectToPlane(B120, B030, vertexNormal[0]);
    
    // get the position of the center (B111)
    glm::vec3 centerOfPNVerts = (B003 + B030 + B300) * ONETHIRD;
    glm::vec3 centerOfMidPoints = (B021 + B012 + B102 +
                              B201 + B210 + B120) * ONESIXTH;
    centerOfMidPoints += (centerOfMidPoints - centerOfPNVerts) * .5f;
    B111 = centerOfMidPoints;
    
}
void Patch::buildPatchNormal() {
    glm::vec3 diffV0 = B030 - B300;
    glm::vec3 diffV1 = B003 - B030;
    glm::vec3 patchNormalRaw = glm::cross(diffV0, diffV1);
    patchNormal = glm::normalize(patchNormalRaw);
}
//glm::vec3 buildPlaneNormal(glm::vec3& diffV0, glm::vec3& diffV1) {
//    // cross product vectors
//    glm::vec3 normal = glm::cross(diffV0, diffV1);
//    return normal;
//}

glm::vec3 Patch::getB030(void) {
    return B030;
}
glm::vec3 Patch::getB021(void) {
    return B021;
}
glm::vec3 Patch::getB012(void) {
    return B012;
}
glm::vec3 Patch::getB003(void) {
    return B003;
}
glm::vec3 Patch::getB102(void) {
    return B102;
}
glm::vec3 Patch::getB201(void) {
    return B201;
}
glm::vec3 Patch::getB300(void) {
    return B300;
}
glm::vec3 Patch::getB210(void) {
    return B210;
}
glm::vec3 Patch::getB120(void) {
    return B120;
}
glm::vec3 Patch::getB111(void) {
    return B111;
}
glm::vec3 Patch::getPatchNormal() {
    return patchNormal;
}
glm::vec3* Patch::getVertexNormal() {
    return vertexNormal;
}
