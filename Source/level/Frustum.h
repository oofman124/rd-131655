#pragma once
#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "../phys/AABB.h"
#include <array>
#include <cmath>
#include <glad/glad.h>


class Frustum {
public:
    // Frustum sides
    enum Side { RIGHT = 0, LEFT, BOTTOM, TOP, BACK, FRONT };
    // Plane coefficients
    enum Plane { A = 0, B, C, D };


    // Get singleton instance (like Java static getFrustum)
    static Frustum& getFrustum();

    // Calculate the frustum from current OpenGL matrices
    void calculateFrustum() {
        float proj[16], modl[16], clip[16];

        glGetFloatv(GL_PROJECTION_MATRIX, proj);
        glGetFloatv(GL_MODELVIEW_MATRIX, modl);

        // Multiply modelview and projection matrices to get the clipping planes
        clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
        clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
        clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
        clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

        clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
        clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
        clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
        clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

        clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
        clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
        clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
        clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

        clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
        clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
        clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
        clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

        // RIGHT
        for (int i = 0; i < 4; ++i) m_Frustum[RIGHT][i] = clip[3 + i*4] - clip[0 + i*4];
        normalizePlane(RIGHT);

        // LEFT
        for (int i = 0; i < 4; ++i) m_Frustum[LEFT][i] = clip[3 + i*4] + clip[0 + i*4];
        normalizePlane(LEFT);

        // BOTTOM
        for (int i = 0; i < 4; ++i) m_Frustum[BOTTOM][i] = clip[3 + i*4] + clip[1 + i*4];
        normalizePlane(BOTTOM);

        // TOP
        for (int i = 0; i < 4; ++i) m_Frustum[TOP][i] = clip[3 + i*4] - clip[1 + i*4];
        normalizePlane(TOP);

        // BACK
        for (int i = 0; i < 4; ++i) m_Frustum[BACK][i] = clip[3 + i*4] - clip[2 + i*4];
        normalizePlane(BACK);

        // FRONT
        for (int i = 0; i < 4; ++i) m_Frustum[FRONT][i] = clip[3 + i*4] + clip[2 + i*4];
        normalizePlane(FRONT);
    }

    // Frustum tests
    bool pointInFrustum(float x, float y, float z) const {
        for (int i = 0; i < 6; ++i)
            if (m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= 0)
                return false;
        return true;
    }

    bool sphereInFrustum(float x, float y, float z, float radius) const {
        for (int i = 0; i < 6; ++i)
            if (m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= -radius)
                return false;
        return true;
    }

    bool cubeInFrustum(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
        for (int i = 0; i < 6; ++i) {
            if (m_Frustum[i][A] * minX + m_Frustum[i][B] * minY + m_Frustum[i][C] * minZ + m_Frustum[i][D] > 0) continue;
            if (m_Frustum[i][A] * maxX + m_Frustum[i][B] * minY + m_Frustum[i][C] * minZ + m_Frustum[i][D] > 0) continue;
            if (m_Frustum[i][A] * minX + m_Frustum[i][B] * maxY + m_Frustum[i][C] * minZ + m_Frustum[i][D] > 0) continue;
            if (m_Frustum[i][A] * maxX + m_Frustum[i][B] * maxY + m_Frustum[i][C] * minZ + m_Frustum[i][D] > 0) continue;
            if (m_Frustum[i][A] * minX + m_Frustum[i][B] * minY + m_Frustum[i][C] * maxZ + m_Frustum[i][D] > 0) continue;
            if (m_Frustum[i][A] * maxX + m_Frustum[i][B] * minY + m_Frustum[i][C] * maxZ + m_Frustum[i][D] > 0) continue;
            if (m_Frustum[i][A] * minX + m_Frustum[i][B] * maxY + m_Frustum[i][C] * maxZ + m_Frustum[i][D] > 0) continue;
            if (m_Frustum[i][A] * maxX + m_Frustum[i][B] * maxY + m_Frustum[i][C] * maxZ + m_Frustum[i][D] > 0) continue;
            return false;
        }
        return true;
    }

    bool cubeInFrustum(const AABB& aabb) const {
        return cubeInFrustum((float)aabb.minX, (float)aabb.minY, (float)aabb.minZ,
                             (float)aabb.maxX, (float)aabb.maxY, (float)aabb.maxZ);
    }

private:
    float m_Frustum[6][4];

    void normalizePlane(int side) {
        float magnitude = std::sqrt(
            m_Frustum[side][A] * m_Frustum[side][A] +
            m_Frustum[side][B] * m_Frustum[side][B] +
            m_Frustum[side][C] * m_Frustum[side][C]);
        m_Frustum[side][A] /= magnitude;
        m_Frustum[side][B] /= magnitude;
        m_Frustum[side][C] /= magnitude;
        m_Frustum[side][D] /= magnitude;
    }
};

#endif // FRUSTUM_H